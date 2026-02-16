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

#include "ConfigParser.h"
#include "ConfigLocator.h"
#include "Core/Log.h"
#include "CppUtils/include/Enum.h"
#include "Keys.h"

using enum_utils::EnumMap;

const EnumMap<Qt::Alignment> alignment_map = {{"top", Qt::AlignTop | Qt::AlignHCenter},
                                              {"center", Qt::AlignCenter},
                                              {"bottom", Qt::AlignBottom | Qt::AlignHCenter},
                                              {"left", Qt::AlignVCenter | Qt::AlignLeft},
                                              {"right", Qt::AlignVCenter | Qt::AlignRight}};

const EnumMap<QSizePolicy> size_policy_map = {{"expanding",
                                               {QSizePolicy::Expanding, QSizePolicy::Expanding}},
                                              {"fixed", {QSizePolicy::Fixed, QSizePolicy::Fixed}}};

// TODO Extract
// TODO Split
// Use textToHexInterpreter on a TOML node to set-up keybindings for target
void interpretTextAsKeybindings(const toml::node_view<const toml::node>& source,
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

// TODO Extract
toml::table createTable(string file_path) {
        toml::table file_table;

        QDEBUG() << file_path;

        try {
                file_table = toml::parse_file(file_path);
        } catch (const toml::parse_error& error) {
                QFATAL("Parsing of %s failed: %s", string(file_path).c_str(),
                       std::string(error.description()).c_str());
        }

        return file_table;
}

std::array<std::string, config_file_names_cnt> config_files = locateConfigFiles();

const toml::table& createConfig() {
        static toml::table config = createTable(config_files[0]);
        return config;
}

const toml::table& createKeys() {
        static toml::table keys = createTable(config_files[1]);
        return keys;
}
