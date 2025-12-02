/* Pretty Applets

   Copyright (C) 2025 Łukasz Wrodarczyk

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
#include "../../modules/CppUtils/include/String.h"
#include "Config.h"
#include "Keys.h"

#include <QDebug>
#include <QFileInfo>
#include <QKeyCombination>
#include <QKeySequence>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <QtEnvironmentVariables>

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdlib>
#include <functional>
#include <iterator>
#include <qnamespace.h>
#include <string>
#include <toml++/impl/value.hpp>
#include <toml++/toml.hpp>
#include <unordered_map>

using std::array;
using std::back_inserter;
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

static const unordered_map<string, Qt::Alignment> alignment_map = {{"top", Qt::AlignTop},
                                                                   {"center", Qt::AlignCenter},
                                                                   {"bottom", Qt::AlignBottom},
                                                                   {"left", Qt::AlignLeft},
                                                                   {"right", Qt::AlignRight}};

static const unordered_map<string, QSizePolicy> size_policy_map =
        {{"expanding", {QSizePolicy::Expanding, QSizePolicy::Expanding}},
         {"fixed", {QSizePolicy::Fixed, QSizePolicy::Fixed}}};

} // namespace

namespace error_message {
namespace alignment {
void textAlignmentError() {
        qWarning() << "Wrong setting in config.toml for: text_alignment"
                   << "Available values: top, center, bottom, left, right"
                   << "Default: top";
}

void iconAlignmentError() {
        qWarning() << "Wrong setting in config.toml for: icon_alignment"
                   << "Available values: top, center, bottom, left, right"
                   << "Default: center";
}
} // namespace alignment
namespace size_policy {
void primaryButtonError() {
        qWarning() << "Wrong setting in config.toml for: policy"
                   << "Available values: expanding, fixed"
                   << "Default: expanding";
}
} // namespace size_policy
} // namespace error_message

// Find value and return default_value if not found
template<typename EnumType>
// TODO Replace std::function with regular function pointer
static EnumType getEnumFromMap(const EnumMap<EnumType>& mapping, const string& key,
                               const EnumType& default_value, std::function<void()> errorMessage) {
        const auto it = mapping.find(key);
        if (it == mapping.end()) {
                errorMessage();
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
                        qCritical()
                                << __func__ << ": Empty source for keybindings:" << keybindings_str;
                } else if (!source.is_array()) {
                        qCritical() << __func__
                                    << ": Non-array source for keybindings:" << keybindings_str;
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
                                                          + "/PrettyApplets/",
                                                  qEnvironmentVariable("XDG_DATA_HOME")
                                                          + "/PrettyApplets/"};
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
                        qFatal("%s: %s not found!", __func__,
                               config_file_names[file_i].toStdString().c_str());
                }
        }

        return files;
}

static toml::table createTable(string file_path) {
        toml::table file_table;

        qDebug() << __func__ << ":" << file_path;

        try {
                file_table = toml::parse_file(file_path);
        } catch (const toml::parse_error& error) {
                qFatal("Parsing of %s failed: %s", string(file_path).c_str(),
                       string(error.description()).c_str());
        }

        return file_table;
}

namespace {
static auto config_files = locateConfigFiles();
}

TomlConfigParser::TomlConfigParser() :
        config_table(createTable(config_files[0])), keys_table(createTable(config_files[1])) {}

TomlConfigParser& TomlConfigParser::getInstance() {
        static TomlConfigParser toml_config_parser;
        return toml_config_parser;
}

// TODO Defaults on failed parsing to avoid crashes
// TODO Create macros for qFatal, qDebug, qInfo, and qCritical that include __func__
// TODO Print the type the element is and shouldn't be
// TODO Apply toLowerCopy where applicable
void TomlConfigParser::parseWindowProperties() {
        const auto window = config_table["global"]["window"].as_table();
        if (!window) { qFatal("%s: global.window needs to be a table!", __func__); }

        // Window size
        const auto size = (*window)["size"].as_array();
        if (!size) {
                qFatal("%s: global.window.size needs to be an array!", __func__);
        } else {
                constexpr size_t       arr_length = 2;
                array<int, arr_length> for_qsize{};

                // TODO Handle insufficient size
                if (arr_length > size->size()) {
                        qFatal("%s: in config.toml, global.window.size requires %zu integers!",
                               __func__, arr_length);
                }

                for (size_t i = 0; i != std::min(size->size(), arr_length); ++i) {
                        const auto index = size->get(i)->as_integer();

                        if (!index) {
                                // TODO Attempt conversion
                                qFatal("%s: in config.toml, global.window.size[%zu] is not an integer!",
                                       __func__, i);
                        }

                        for_qsize[i] = index->get();
                }

                Config::WindowProperties::size = QSize(for_qsize[0], for_qsize[1]);
        }

        // Window title
        const auto title = (*window)["title"].as_string();
        if (!title) {
                qFatal("%s: in config.toml, global.window.title is not a string!", __func__);
        }

        Config::WindowProperties::title = QString::fromStdString(title->get());
}

void TomlConfigParser::parseButtonProperties() {
        const auto button = config_table["global"]["primary_button"].as_table();
        if (!button) {
                qFatal("%s: in config.toml, global.primary_button is not a table!", __func__);
        }

        // Text alignment
        const auto text_alignment = (*button)["text_alignment"].as_string();
        if (!text_alignment) {
                qFatal("%s: in config.toml, global.primary_button.text_alignment is not a string!",
                       __func__);
        } else {
                Qt::Alignment default_alignment = Qt::AlignTop;

                // TODO Top results in top-left alignment, determine why
                Config::PrimaryButtonProperties::text_alignment =
                        getEnumFromMap(alignment_map, text_alignment->get(), default_alignment,
                                       error_message::alignment::textAlignmentError);
        }

        // Icon alignment
        const auto icon_alignment = (*button)["icon_alignment"].as_string();
        if (!icon_alignment) {
                qFatal("%s: in config.toml, global.primary_button.icon_alignment is not a string!",
                       __func__);
        } else {
                Qt::Alignment default_alignment = Qt::AlignCenter;

                Config::PrimaryButtonProperties::icon_alignment =
                        getEnumFromMap(alignment_map, icon_alignment->get(), default_alignment,
                                       error_message::alignment::textAlignmentError);
        }

        // Icon size
        const auto icon_size = (*button)["icon_size"].as_array();
        if (!icon_size) {
                qFatal("%s: in config.toml, global.primary_button.icon_size is not an array!",
                       __func__);
        } else {
                constexpr size_t       arr_length = 2;
                array<int, arr_length> for_qsize{};

                // TODO Handle insufficient size
                if (arr_length > icon_size->size()) {
                        qFatal("%s: in config.toml, global.primary_button.icon_size requires %zu integers!",
                               __func__, arr_length);
                }

                for (size_t i = 0; i != std::min(icon_size->size(), arr_length); ++i) {
                        const auto index = icon_size->get(i)->as_integer();
                        if (!index) {
                                qFatal("%s: in config.toml, global.primary_button.icon_size[%zu] is not an integer!",
                                       __func__, i);
                        }

                        for_qsize[i] = index->get();
                }

                Config::PrimaryButtonProperties::icon_size = QSize(for_qsize[0], for_qsize[1]);
        }

        // Policy
        const auto policy = (*button)["policy"].as_string();
        if (!policy) {
                qFatal("%s: in config.toml, global.primary_button.policy is not a string!",
                       __func__);
        } else {
                const QSizePolicy default_policy = QSizePolicy(QSizePolicy::Expanding,
                                                               QSizePolicy::Expanding);
                Config::PrimaryButtonProperties::policy =
                        getEnumFromMap(size_policy_map, toLowerCopy(policy->get()), default_policy,
                                       error_message::size_policy::primaryButtonError);
        }
}

void TomlConfigParser::parseLayoutProperties() {
        const auto layout = config_table["power_applet"]["layout"].as_table();
        if (!layout) {
                qFatal("%s: in config.toml, power_applet.layout is not a table!", __func__);
        }

        // Primary power buttons
        const auto primary_buttons = (*layout)["primary_buttons"].as_array();
        if (!primary_buttons) {
                qFatal("%s: in config.toml, power_applet.layout.primary_buttons is not a table!",
                       __func__);
        }

        vector<PrimaryButtonData> power_buttons{};

        const char* func_str = __func__;

        // TODO Extract the parser here
        // TODO Returns in all paths
        transform(
                primary_buttons->begin(), primary_buttons->end(), back_inserter(power_buttons),
                [&primary_buttons, func_str](const toml::node& node) -> PrimaryButtonData {
                        // Find position of node in primary_buttons
                        const auto it = std::find_if(primary_buttons->begin(),
                                                     primary_buttons->end(),
                                                     [&node](const toml::node& n) -> bool {
                                                             return &n == &node;
                                                     });

                        const size_t index = std::distance(primary_buttons->begin(), it);

                        PrimaryButtonData button_data{};

                        const auto button = node.as_table();
                        if (!button) {
                                qFatal("%s: in config.toml, power_applet.layout.primary_buttons[%zu] is not a table!",
                                       func_str, index);
                        }

                        const auto enabled_opt = (*button)["enabled"].as_boolean();
                        if (!enabled_opt) {
                                qFatal("%s: in config.toml, power_applet.layout.primary_buttons[%zu].enabled is not a boolean!",
                                       __func__, index);
                        } else {
                                bool enabled = enabled_opt->value_or(true);
                                if (enabled) {
                                        const auto id = (*button)["id"].as_string();
                                        if (!id) {
                                                qFatal("%s: in config.toml, power_applet.layout.primary_buttons[%zu].id is not a string!",
                                                       __func__, index);
                                        } else {
                                                button_data.identifier = QString::fromStdString(
                                                        id->get());
                                        }

                                        const auto label = (*button)["label"].as_string();
                                        if (!label) {
                                                qFatal("%s: in config.toml, power_applet.layout.primary_buttons[%zu].label is not a string!",
                                                       __func__, index);
                                        } else {
                                                button_data.text = QString::fromStdString(
                                                        label->get());
                                        }

                                        const auto order = (*button)["order"].as_integer();
                                        if (!order) {
                                                qFatal("%s: in config.toml, power_applet.layout.primary_buttons[%zu].order is not an integer!",
                                                       __func__, index);
                                        } else {
                                                button_data.order = order->get();
                                        }

                                        return std::move(button_data);
                                } else {
                                        const auto id = (*button)["id"].as_string();
                                        if (!id) {
                                                qFatal("%s: in config.toml, power_applet.layout.primary_buttons[%zu].id is not a string!",
                                                       __func__, index);
                                        }

                                        qDebug() << (*button)["id"].as_string()->get()
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

void TomlConfigParser::parseConfig() {
        // Check the validity of global and power_applet
        const auto global = config_table["global"].as_table();
        if (!global) { qFatal("%s: in config.toml, global is not a table!", __func__); }

        const auto power_applet = config_table["power_applet"].as_table();
        if (!power_applet) { qFatal("%s: in config.toml, power_applet is not a table!", __func__); }

        /* Window properties */
        parseWindowProperties();

        /* Button properties */
        parseButtonProperties();

        /* Window layout properties */
        parseLayoutProperties();

        /* Keys */
        parseKeys();
}
