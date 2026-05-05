// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "KeysMapper.h"
#include "Applets/Types/AppletRecord.h"
#include "Config/KeysFile/Types/Keybindings.h"
#include "Config/Resolver/Resolver.h"
#include "Config/Types/NodePair.h"
#include "Config/Types/NodeView.h"
#include "CppUtils/Log/QtLog.h"

#include <cstddef>
#include <optional>
#include <string>
#include <toml++/toml.hpp>
#include <utility>
#include <vector>
#include <QApplication>
#include <QKeyCombination>
#include <QKeySequence>
#include <QString>

int interpretTextAsHex(const std::string& text) {
        QKeySequence    sequence(QString::fromStdString(text));
        QKeyCombination combination(sequence[0]);

        return combination.key();
};

keybindings interpretTextAsKeybindings(const std::vector<std::string>& text_list) {
        keybindings keys{};
        keys.reserve(text_list.size());
        for (const std::string& text : text_list) { keys.insert(interpretTextAsHex(text)); }

        return keys;
}

std::vector<std::string> interpretTomlArrayAsStringVector(const toml::array& toml_array) {
        std::vector<std::string> str_vec{};
        str_vec.reserve(toml_array.size());

        for (const auto& element : toml_array) {
                if (const auto& str_element = element.as_string()) {
                        str_vec.push_back(str_element->get());
                }
        }

        return str_vec;
}

void KeysMapper::mapQuitKeys(NodePair nodes, keybindings& quit, const keybindings& defaults,
                             const PathContext& path_context) {
        constexpr size_t min_size = 1;

        toml::array array{};
        Resolver::fromOrDefault<toml::array>({Source{nodes.primary, applet::power_applet.scope},
                                              Source{nodes.fallback, applet::global.scope}},
                                             array, quit, defaults, path_context, {min_size},
                                             "Format: [keybindings...]");

        quit = interpretTextAsKeybindings(interpretTomlArrayAsStringVector(array));
}

void KeysMapper::mapPrimaryButtonKey(node_view primary_button_node, keybindings& primary_button,
                                     const keybindings& defaults, const PathContext& path_context) {
        constexpr size_t min_size = 1;

        toml::array button{};
        Resolver::fromOrDefault<toml::array>({Source{primary_button_node,
                                                     applet::power_applet.scope}},
                                             button, primary_button, defaults, path_context,
                                             {min_size}, "Format: [keybindings...]");

        primary_button = interpretTextAsKeybindings(interpretTomlArrayAsStringVector(button));
}

void KeysMapper::mapPrimaryButtonKeys(node_view                       primary_buttons_node,
                                      std::vector<keybindings>&       primary_buttons,
                                      const std::vector<keybindings>& defaults,
                                      const PathContext&              path_context) {
        constexpr size_t min_size = 1;
        const size_t     max_size = primary_buttons.size();

        toml::array primary_button_arr{};
        Resolver::fromOrDefault({Source{primary_buttons_node, applet::power_applet.scope}},
                                primary_button_arr, primary_buttons, defaults, path_context,
                                {min_size}, "Format: [keybindings...]");

        std::vector<keybindings> primary_buttons_new{};
        for (size_t i = 0; i != primary_button_arr.size(); ++i) {
                keybindings keys{};
                mapPrimaryButtonKey(primary_buttons_node[i], keys, defaults[i],
                                    path_context.getExtended(i));
                if (!keys.empty()) { primary_buttons_new.push_back(keys); }
        };

        primary_buttons = std::move(primary_buttons_new);
}
