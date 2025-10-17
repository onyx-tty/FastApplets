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
#include "../Environment/RootResolver.h"
#include "Config.h"
#include "Keys.h"

#include <QDebug>
#include <QKeyCombination>
#include <QKeySequence>
#include <QSize>
#include <QSizePolicy>

#include <algorithm>
#include <cctype>
#include <iterator>
#include <qnamespace.h>
#include <string>
#include <string_view>
#include <toml++/impl/value.hpp>
#include <toml++/toml.hpp>

using std::back_inserter;
using std::inserter;
using std::string;
using std::string_view;
using std::transform;

static void tolower(string& str) {
        for (char& ch : str) { ch = std::tolower(static_cast<unsigned char>(ch)); }
}

static string tolower(const string& str) {
        string new_str = str;

        tolower(new_str);

        return new_str;
}

// Defer fetching project root until runtime, as RootResolver::getInstance().getProjectRoot() calls
// QApplication. Evaluating the variable at compile time would lead to a failure.
static string& projectRoot() {
        static string project_root = RootResolver::getInstance().getProjectRoot().toStdString();

        return project_root;
}

static string configLocation() {
        string config_location = projectRoot() + "/config/config.toml";

        return config_location;
}

static string keysLocation() {
        string keys_location = projectRoot() + "/config/keys.toml";

        return keys_location;
}

static toml::table createTable(string_view file_path) {
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

TomlConfigParser::TomlConfigParser() :
        config_table(createTable(configLocation())), keys_table(createTable(keysLocation())) {}

TomlConfigParser& TomlConfigParser::getInstance() {
        static TomlConfigParser toml_config_parser;
        return toml_config_parser;
}

void TomlConfigParser::parseConfig() {
        /* Window properties */
        // Window size
        const auto window_size_raw     = config_table["global"]["window"]["size"].as_array();
        Config::WindowProperties::size = QSize(window_size_raw->get(0)->as_integer()->get(),
                                               window_size_raw->get(1)->as_integer()->get());

        // Window title
        Config::WindowProperties::title = QString::fromStdString(
                config_table["global"]["window"]["title"].as_string()->get());

        /* Window layout properties */
        // Primary power buttons
        const auto primary_power_buttons = config_table["power_applet"]["layout"]["primary_buttons"]
                                                   .as_array();
        transform(primary_power_buttons->begin(), primary_power_buttons->end(),
                  back_inserter(Config::WindowLayoutProperties::primary_power_buttons),
                  [](const toml::node& node) -> PrimaryButtonData {
                          const auto node_arr = node.as_array();
                          // {identifier, text}
                          return {QString::fromStdString(node_arr->get(0)->as_string()->get()),
                                  QString::fromStdString(node_arr->get(1)->as_string()->get())};
                  });

        /* Button properties */
        // Text alignment
        const auto text_alignment_raw =
                config_table["global"]["primary_button"]["text_alignment"].as_string()->get();

        // TODO Top results in top-left alignment, determine why
        if (text_alignment_raw == "top") {
                Config::PrimaryButtonProperties::text_alignment = Qt::AlignTop;
        } else if (text_alignment_raw == "center") {
                Config::PrimaryButtonProperties::text_alignment = Qt::AlignCenter;
        } else if (text_alignment_raw == "bottom") {
                Config::PrimaryButtonProperties::text_alignment = Qt::AlignBottom;
        } else if (text_alignment_raw == "left") {
                Config::PrimaryButtonProperties::text_alignment = Qt::AlignLeft;
        } else if (text_alignment_raw == "right") {
                Config::PrimaryButtonProperties::text_alignment = Qt::AlignBottom;
        } else {
                qWarning() << "Wrong setting in config.toml for: text_alignment"
                           << "Available values: top, center, bottom, left, right"
                           << "Default: top";
                Config::PrimaryButtonProperties::text_alignment = Qt::AlignTop; // default to top
        }

        // Icon alignment
        const auto icon_alignment_raw =
                config_table["global"]["primary_button"]["icon_alignment"].as_string()->get();

        if (icon_alignment_raw == "top") {
                Config::PrimaryButtonProperties::icon_alignment = Qt::AlignTop;
        } else if (icon_alignment_raw == "center") {
                Config::PrimaryButtonProperties::icon_alignment = Qt::AlignCenter;
        } else if (icon_alignment_raw == "bottom") {
                Config::PrimaryButtonProperties::icon_alignment = Qt::AlignBottom;
        } else if (icon_alignment_raw == "left") {
                Config::PrimaryButtonProperties::icon_alignment = Qt::AlignLeft;
        } else if (icon_alignment_raw == "right") {
                Config::PrimaryButtonProperties::icon_alignment = Qt::AlignBottom;
        } else {
                qWarning() << "Wrong setting in config.toml for: icon_alignment"
                           << "Available values: top, center, bottom, left, right"
                           << "Default: top";
                Config::PrimaryButtonProperties::icon_alignment = Qt::AlignCenter;
        }

        // Icon size
        const auto icon_size_raw = config_table["global"]["primary_button"]["icon_size"].as_array();
        Config::PrimaryButtonProperties::icon_size =
                QSize(icon_size_raw->get(0)->as_integer()->get(),
                      icon_size_raw->get(1)->as_integer()->get());

        // Policy
        const auto policy_raw = config_table["global"]["primary_button"]["policy"].as_string()->get();
        if (tolower(policy_raw) == "expanding") {
                Config::PrimaryButtonProperties::policy = QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        } else if (tolower(policy_raw) == "fixed") {
                Config::PrimaryButtonProperties::policy = QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        } else {
                qWarning() << "Wrong setting in config.toml for: policy"
                           << "Available values: expanding, fixed"
                           << "Default: expanding";
                Config::PrimaryButtonProperties::policy = QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        }

        /* Keys */
        // Turn a node containing strings representing a keybinding, i.e. Q, E, M, Escape
        // into a corresponding hex number representing a Qt::Key enum value.
        const auto keybindingTextToHex = [](const auto& node) -> int {
                QKeySequence    sequence(QString::fromStdString(node.as_string()->get()));
                QKeyCombination combination(sequence[0]);

                return combination.key();
        };

        // Automate processing nodes and filling target 'keybindings' variables with data
        // obtained from keybindingTextToHex
        const auto translateKeybindings =
                [&keybindingTextToHex](const toml::node_view<const toml::node>& source,
                                       keybindings&                             target) {
                if (!source || !source.is_array()) {
                        QString keybindings_str = "";
                        for (const auto& key : target) {
                                if (!keybindings_str.isEmpty()) {
                                        keybindings_str += ",";
                                } 

                                keybindings_str += QString::number(key);
                        }
                        
                        if (!source) {
                                qCritical() << __func__ << ": Empty source for keybindings:"
                                            << keybindings_str;
                        } else if (!source.is_array()) {
                                qCritical() << __func__ << ": Non-array source for keybindings:"
                                            << keybindings_str;
                        }

                        return; // Drop these keybindings if source doesn't exist
                }
                const auto keys_raw = source.as_array();

                target.reserve(keys_raw->size());
                transform(keys_raw->begin(), keys_raw->end(), inserter(target, target.begin()),
                          keybindingTextToHex);
                };

        // Quit
        translateKeybindings(keys_table["global"]["quit"], Keys::GlobalKeys::quit_keys);

        // Primary button control keys - PowerApplet
        translateKeybindings(keys_table["power_applet"]["primary_button1"],
                             Keys::PowerAppletKeys::primary_button1_keys);
        translateKeybindings(keys_table["power_applet"]["primary_button2"],
                             Keys::PowerAppletKeys::primary_button2_keys);
        translateKeybindings(keys_table["power_applet"]["primary_button3"],
                             Keys::PowerAppletKeys::primary_button3_keys);
        translateKeybindings(keys_table["power_applet"]["primary_button4"],
                             Keys::PowerAppletKeys::primary_button4_keys);
}
