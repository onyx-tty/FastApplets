/* FastApplets

   Copyright (C) 2026 Łukasz Wrodarczyk

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#include "TomlConfigParser.h"
#include "Config.h"
#include "Core/Log.h"
#include "CppUtils/include/String.h"
#include "Keys.h"

#include <QFileInfo>
#include <QKeyCombination>
#include <QKeySequence>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <QtEnvironmentVariables>

#include <algorithm>
#include <array>
#include <cstdlib>
#include <iterator>
#include <qnamespace.h>
#include <string>
#include <toml++/impl/value.hpp>
#include <toml++/toml.hpp>
#include <unordered_map>

using std::array;
using std::for_each;
using std::inserter;
using std::sort;
using std::string;
using std::to_string;
using std::transform;
using std::unordered_map;
using std::vector;
using string_utils::toLowerCopy;

template<typename EnumType>
using EnumMap = unordered_map<string, EnumType>;

namespace {

static const unordered_map<string, Qt::Alignment> alignment_map =
        {{"top", Qt::AlignTop | Qt::AlignHCenter},
         {"center", Qt::AlignCenter},
         {"bottom", Qt::AlignBottom | Qt::AlignHCenter},
         {"left", Qt::AlignVCenter | Qt::AlignLeft},
         {"right", Qt::AlignVCenter | Qt::AlignRight}};

static const unordered_map<string, QSizePolicy> size_policy_map =
        {{"expanding", {QSizePolicy::Expanding, QSizePolicy::Expanding}},
         {"fixed", {QSizePolicy::Fixed, QSizePolicy::Fixed}}};

namespace error_message {
namespace alignment {
// TODO Simply store QString instead of serving as a functor
// clang-format off
QString text_alignment_error =
        "Wrong setting in config.toml for: text_alignment\n"
        "Available values: top, center, bottom, left, right\n"
        "Default: top";
// clang-format on

// clang-format off
QString icon_alignment_error =
        "Wrong setting in config.toml for: icon_alignment"
        "Available values: top, center, bottom, left, right"
        "Default: center";
// clang-format on

} // namespace alignment
namespace size_policy {
// clang-format off
QString primary_button_error =
        "Wrong setting in config.toml for: policy"
        "Available values: expanding, fixed"
        "Default: expanding";
// clang-format on
} // namespace size_policy
} // namespace error_message

} // namespace

// Find value and return default_value if not found
template<typename EnumType>
static EnumType getEnumFromMap(const EnumMap<EnumType>& mapping, const string& key,
                               const EnumType& default_value, QString error_message) {
        const auto it = mapping.find(key);
        if (it == mapping.end()) {
                QWARNING() << error_message;
                return default_value;
        }

        return it->second;
}

// Interpret keybinding text as a corresponding hexadecimal value for the Qt::Key enum
static const auto textToHexInterpreter = [](const auto& node) {
        QKeySequence    sequence(QString::fromStdString(node.as_string()->get()));
        QKeyCombination combination(sequence[0]);

        return combination.key();
};

// Use textToHexInterpreter on a TOML node to set-up keybindings for target
static void interpretTextAsKeybindings(const toml::node_view<const toml::node>& source,
                                       keybindings&                             target) {
        if (!source || !source.is_array()) {
                QString keybindings_str = "";
                for (const auto& key : target) {
                        if (!keybindings_str.isEmpty()) { keybindings_str += ","; }

                        keybindings_str += QString::number(key);
                }

                if (!source) {
                        QCRITICAL() << "Empty source for keybindings:" << keybindings_str;
                } else if (!source.is_array()) {
                        QCRITICAL() << ": Non-array source for keybindings:" << keybindings_str;
                }

                return; // Drop these keybindings if source doesn't exist
        }
        const auto keys_raw = source.as_array();

        target.reserve(keys_raw->size());
        transform(keys_raw->begin(), keys_raw->end(), inserter(target, target.begin()),
                  textToHexInterpreter);
};

// Values needed to find configs
namespace {

constexpr int            config_dir_paths_cnt  = 2;
constexpr int            config_file_names_cnt = 2;
static array<QString, 2> config_dir_paths      = {qEnvironmentVariable("XDG_CONFIG_HOME")
                                                          + "/FastApplets/",
                                                  qEnvironmentVariable("XDG_DATA_HOME")
                                                          + "/FastApplets/"};
static array<QString, config_file_names_cnt> config_file_names = {"config.toml", "keys.toml"};

} // namespace

// Look for configs in $XDG_CONFIG_HOME and $XDG_DATA_HOME
static array<string, config_file_names_cnt> locateConfigFiles() {
        array<string, config_file_names_cnt> files{}; // Only enough slots for each file

        QString file_path;
        // Loop through expected config files
        for (size_t file_i = 0; file_i != config_file_names_cnt; ++file_i) {
                bool found = false;
                // Loop through expected directories
                for (size_t dir_i = 0; !found && dir_i != config_dir_paths_cnt; ++dir_i) {
                        file_path = config_dir_paths[dir_i] + config_file_names[file_i];
                        // If file found, save filepath, stop the loop for that file
                        if (QFileInfo::exists(file_path)) {
                                files[file_i] = file_path.toStdString();
                                found         = true;
                                break;
                        }
                }

                // No valid file_path found for the current file, terminate
                // TODO Generate default toml file on failure
                if (!found) {
                        QFATAL("%s not found!", config_file_names[file_i].toStdString().c_str());
                }
        }

        return files;
}

static toml::table createTable(string file_path) {
        toml::table file_table;

        QDEBUG() << file_path;

        try {
                file_table = toml::parse_file(file_path);
        } catch (const toml::parse_error& error) {
                QFATAL("Parsing of %s failed: %s", string(file_path).c_str(),
                       string(error.description()).c_str());
        }

        return file_table;
}

namespace {

static auto config_files = locateConfigFiles();

} // namespace

TomlConfigParser::TomlConfigParser() :
        config_table(createTable(config_files[0])), keys_table(createTable(config_files[1])) {}

TomlConfigParser& TomlConfigParser::getInstance() {
        static TomlConfigParser toml_config_parser;
        return toml_config_parser;
}

// TODO Detect mismatched types, log them. Example: "expected int but got string"
// TODO Apply toLowerCopy where applicable
void TomlConfigParser::parseWindowProperties() {
        // TODO Defaults
        const auto window = config_table["global"]["window"].as_table();
        if (!window) { QFATAL("global.window needs to be a table!"); }

        // Window size
        // TODO size_scale config option to let the size be a % of screen size
        // TODO option to automatically detect and assign monitor size to size and then multiply by size_scale
        const auto size = (*window)["size"].as_array();
        if (!size) {
                // TODO Defaults
                QFATAL("global.window.size needs to be an array!");
        } else {
                constexpr size_t       arr_length = 2;
                array<int, arr_length> for_qsize{};

                // TODO Default missing values, pass the rest
                if (arr_length > size->size()) {
                        QFATAL("in config.toml, global.window.size requires %zu integers!",
                               arr_length);
                }

                for (size_t i = 0; i != std::min(size->size(), arr_length); ++i) {
                        const auto index = size->get(i)->as_integer();

                        if (!index) {
                                const auto str_index = size->get(i)->as_string();

                                QWARNING_NS()
                                        << "in config.toml, global.window.size[" << i
                                        << "] is not an integer! Attempting conversion from string to integer...";
                                if (!str_index) {
                                        // TODO Defaults
                                        QFATAL("FAILED CONVERSION!");
                                } else {
                                        for_qsize[i] = std::stoi(str_index->get());
                                        QINFO() << "Successful conversion!";
                                }
                        } else {
                                for_qsize[i] = index->get();
                        }
                }

                Config::WindowProperties::size = QSize(for_qsize[0], for_qsize[1]);
        }

        // Window title
        // TODO Convert to string
        // TODO Defaults
        const auto title = (*window)["title"].as_string();
        if (!title) { QFATAL("in config.toml, global.window.title is not a string!"); }

        Config::WindowProperties::title = QString::fromStdString(title->get());
}

void TomlConfigParser::parseButtonProperties() {
        const auto button = config_table["global"]["primary_button"].as_table();
        if (!button) { QFATAL("in config.toml, global.primary_button is not a table!"); }

        // Text alignment
        const auto text_alignment = (*button)["text_alignment"].as_string();
        if (!text_alignment) {
                // TODO Defaults
                QFATAL("in config.toml, global.primary_button.text_alignment is not a string!");
        } else {
                Qt::Alignment default_alignment = alignment_map.at("top");

                Config::PrimaryButtonProperties::text_alignment =
                        getEnumFromMap(alignment_map, text_alignment->get(), default_alignment,
                                       error_message::alignment::text_alignment_error);
        }

        // Icon alignment
        // TODO This option doesn't work, fix
        const auto icon_alignment = (*button)["icon_alignment"].as_string();
        if (!icon_alignment) {
                // TODO Defaults
                QFATAL("in config.toml, global.primary_button.icon_alignment is not a string!");
        } else {
                Qt::Alignment default_alignment = alignment_map.at("top");

                Config::PrimaryButtonProperties::icon_alignment =
                        getEnumFromMap(alignment_map, icon_alignment->get(), default_alignment,
                                       error_message::alignment::icon_alignment_error);
        }

        // Icon size
        const auto icon_size = (*button)["icon_size"].as_array();
        if (!icon_size) {
                // TODO If icon_size is int, apply to both. If not, try to convert. If fails, default.
                // TODO Defaults
                QFATAL("in config.toml, global.primary_button.icon_size is not an array!");
        } else {
                constexpr size_t       arr_length = 2;
                array<int, arr_length> for_qsize{};

                if (arr_length > icon_size->size()) {
                        // TODO If icon_size[0] exists and is int, apply to both. If not, try to convert. If fails, default.
                        // TODO Defaults
                        QFATAL("in config.toml, global.primary_button.icon_size requires %zu integers!",
                               arr_length);
                }

                for (size_t i = 0; i != std::min(icon_size->size(), arr_length); ++i) {
                        const auto index = icon_size->get(i)->as_integer();
                        if (!index) {
                                // TODO If icon_size[0] exists and is int, apply to both. If not, try to convert. If fails, default.
                                // TODO Defaults
                                QFATAL("in config.toml, global.primary_button.icon_size[%zu] is not an integer!",
                                       i);
                        }

                        for_qsize[i] = index->get();
                }

                Config::PrimaryButtonProperties::icon_size = QSize(for_qsize[0], for_qsize[1]);
        }

        // Policy
        const auto policy = (*button)["policy"].as_string();
        if (!policy) {
                // TODO Defaults
                QFATAL("in config.toml, global.primary_button.policy is not a string!");
        } else {
                const QSizePolicy default_policy = QSizePolicy(QSizePolicy::Expanding,
                                                               QSizePolicy::Expanding);
                Config::PrimaryButtonProperties::policy =
                        getEnumFromMap(size_policy_map, toLowerCopy(policy->get()), default_policy,
                                       error_message::size_policy::primary_button_error);
        }
}

void TomlConfigParser::parseLayoutProperties() {
        const auto layout = config_table["power_applet"]["layout"].as_table();
        // TODO Defaults
        if (!layout) { QFATAL("in config.toml, power_applet.layout is not a table!"); }

        // Primary power buttons
        const auto primary_buttons = (*layout)["primary_buttons"].as_array();
        if (!primary_buttons) {
                // TODO Defaults
                QFATAL("in config.toml, power_applet.layout.primary_buttons is not a table!");
        }

        vector<PrimaryButtonData> power_buttons{};

        // TODO Extract the parser here
        // TODO Split and simplify
        for_each(
                primary_buttons->begin(), primary_buttons->end(),
                [&primary_buttons, &power_buttons](const toml::node& node) {
                        // Find position of node in primary_buttons
                        // TODO Simplify, find_if I think is redundant
                        const auto it = std::find_if(primary_buttons->begin(),
                                                     primary_buttons->end(),
                                                     [&node](const toml::node& n) -> bool {
                                                             return &n == &node;
                                                     });

                        const size_t index = std::distance(primary_buttons->begin(), it);

                        PrimaryButtonData button_data{};

                        const auto button = node.as_table();
                        if (!button) {
                                // TODO Defaults
                                QFATAL("in config.toml, power_applet.layout.primary_buttons[%zu] is not a table!",
                                       index);
                        }

                        const auto enabled_opt = (*button)["enabled"].as_boolean();
                        if (!enabled_opt) {
                                // TODO Attempt conversion. If fails, default.
                                // TODO Defaults
                                QFATAL("in config.toml, power_applet.layout.primary_buttons[%zu].enabled is not a boolean!",
                                       index);
                        } else {
                                bool enabled = enabled_opt->value_or(true);
                                if (enabled) {
                                        const auto id = (*button)["id"].as_string();
                                        if (!id) {
                                                // TODO Defaults
                                                QFATAL("in config.toml, power_applet.layout.primary_buttons[%zu].id is not a string!",
                                                       index);
                                        } else {
                                                button_data.identifier = QString::fromStdString(
                                                        id->get());
                                        }

                                        const auto label = (*button)["label"].as_string();
                                        if (!label) {
                                                // TODO Attempt conversion. If fails, default.
                                                // TODO Defaults
                                                QFATAL("in config.toml, power_applet.layout.primary_buttons[%zu].label is not a string!",
                                                       index);
                                        } else {
                                                button_data.text = QString::fromStdString(
                                                        label->get());
                                        }

                                        const auto order = (*button)["order"].as_integer();
                                        if (!order) {
                                                // TODO If order exists and is not int, try to convert. If fails, default.
                                                // TODO Defaults
                                                QFATAL("in config.toml, power_applet.layout.primary_buttons[%zu].order is not an integer!",
                                                       index);
                                        } else {
                                                button_data.order = order->get();
                                        }

                                        power_buttons.push_back(std::move(button_data));
                                } else {
                                        const auto id = (*button)["id"].as_string();
                                        if (!id) { // TODO Duplication, remove
                                                // TODO Defaults
                                                QFATAL("in config.toml, power_applet.layout.primary_buttons[%zu].id is not a string!",
                                                       index);
                                        }

                                        QDEBUG() << (*button)["id"].as_string()->get()
                                                 << ": DISABLED";
                                }
                        }
                });

        // TODO Handle multiple order integers of the same value
        sort(power_buttons.begin(), power_buttons.end(),
             [](const PrimaryButtonData& a, const PrimaryButtonData& b) -> bool {
                     return a.order < b.order;
             });

        Config::WindowLayoutProperties::primary_power_buttons = std::move(power_buttons);
}

void TomlConfigParser::parseKeys() {
        // interpretTextAsKeybindings already checks for validity
        interpretTextAsKeybindings(keys_table["global"]["quit"], Keys::GlobalKeys::quit_keys);
        interpretTextAsKeybindings(keys_table["power_applet"]["quit"],
                                   Keys::PowerAppletKeys::quit_keys);

        if (Keys::PowerAppletKeys::quit_keys.empty()) { // TODO std::variant<Keybindings, Keybindings&>
                Keys::PowerAppletKeys::quit_keys = Keys::GlobalKeys::quit_keys;
        }

        // Primary button control keys - PowerApplet
        for (size_t i = 0; i != Keys::PowerAppletKeys::primary_button_keys.size(); ++i) {
                string button_name = "primary_button" + to_string(i + 1);
                interpretTextAsKeybindings(keys_table["power_applet"][button_name],
                                           Keys::PowerAppletKeys::primary_button_keys[i]);
        }
}

// TODO Split between a parser for Config.toml and Keys.toml
void TomlConfigParser::parseConfig() {
        // Check the validity of global and power_applet
        const auto global = config_table["global"].as_table();
        // TODO Defaults
        if (!global) { QFATAL("in config.toml, global is not a table!"); }

        const auto power_applet = config_table["power_applet"].as_table();
        // TODO Defaults
        if (!power_applet) { QFATAL("in config.toml, power_applet is not a table!"); }

        /* Window properties */
        parseWindowProperties();

        /* Button properties */
        parseButtonProperties();

        /* Window layout properties */
        parseLayoutProperties();

        /* Keys */
        parseKeys();
}
