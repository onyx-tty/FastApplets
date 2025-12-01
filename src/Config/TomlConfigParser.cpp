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

// TODO Map for config_table and keys_table
void TomlConfigParser::parseConfig() {
        /* Window properties */
        // Window size
        const auto window_size_raw     = config_table["global"]["window"]["size"].as_array();
        Config::WindowProperties::size = QSize(window_size_raw->get(0)->as_integer()->get(),
                                               window_size_raw->get(1)->as_integer()->get());

        // Window title
        Config::WindowProperties::title = QString::fromStdString(
                config_table["global"]["window"]["title"].as_string()->get());

        /* Button properties */
        // Text alignment
        const auto text_alignment_raw =
                config_table["global"]["primary_button"]["text_alignment"].as_string()->get();
        Qt::Alignment default_alignment = Qt::AlignTop;

        // TODO Top results in top-left alignment, determine why
        Config::PrimaryButtonProperties::text_alignment =
                getEnumFromMap(alignment_map, text_alignment_raw, default_alignment,
                               error_message::alignment::textAlignmentError);

        // Icon alignment
        const auto icon_alignment_raw =
                config_table["global"]["primary_button"]["icon_alignment"].as_string()->get();
        default_alignment = Qt::AlignCenter;

        Config::PrimaryButtonProperties::icon_alignment =
                getEnumFromMap(alignment_map, icon_alignment_raw, default_alignment,
                               error_message::alignment::textAlignmentError);

        // Icon size
        const auto icon_size_raw = config_table["global"]["primary_button"]["icon_size"].as_array();
        Config::PrimaryButtonProperties::icon_size =
                QSize(icon_size_raw->get(0)->as_integer()->get(),
                      icon_size_raw->get(1)->as_integer()->get());

        // Policy
        const auto policy_raw = config_table["global"]["primary_button"]["policy"].as_string()->get();
        const QSizePolicy default_policy = QSizePolicy(QSizePolicy::Expanding,
                                                       QSizePolicy::Expanding);
        Config::PrimaryButtonProperties::policy =
                getEnumFromMap(size_policy_map, toLowerCopy(policy_raw), default_policy,
                               error_message::size_policy::primaryButtonError);

        /* Window layout properties */
        // Primary power buttons
        const auto primary_power_buttons_ct =
                config_table["power_applet"]["layout"]["primary_buttons"].as_array();
        vector<PrimaryButtonData> primary_power_buttons{};
        transform(primary_power_buttons_ct->begin(), primary_power_buttons_ct->end(),
                  back_inserter(primary_power_buttons),
                  [](const toml::node& button_node) -> PrimaryButtonData {
                          const auto button_node_table = button_node.as_table();
                          bool       enabled = (*button_node_table)["enabled"].value_or(true);

                          if (enabled) {
                                  return {QString::fromStdString(
                                                  (*button_node_table)["id"].as_string()->get()),
                                          QString::fromStdString(
                                                  (*button_node_table)["label"].as_string()->get()),
                                          (*button_node_table)["order"].as_integer()->get()};
                          } else {
                                  qDebug() << (*button_node_table)["id"].as_string()->get()
                                           << ": DISABLED";
                          }
                  });

        // TODO Handle multiple order integers of the same value
        sort(primary_power_buttons.begin(), primary_power_buttons.end(),
             [](const PrimaryButtonData& a, const PrimaryButtonData& b) -> bool {
                     return a.order < b.order;
             });

        Config::WindowLayoutProperties::primary_power_buttons = std::move(primary_power_buttons);

        /* Keys */
        // Quit
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
