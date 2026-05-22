// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "KeysMapper.h"
#include "Applets/Types/AppletRecord.h"
#include "Config/KeysFile/Types/Keybindings.h"
#include "Config/Resolver/PathContext/PathContext.h"
#include "Config/Resolver/Resolver.h"
#include "Config/Resolver/Types/Source.h"
#include "Config/Types/NodePair.h"
#include "Config/Types/NodeView.h"

#include <cstddef>
#include <string>
#include <toml++/toml.hpp>
#include <utility>
#include <vector>
#include <QApplication>
#include <QKeySequence>
#include <QString>

keybindings keysFromText(const std::vector<std::string>& texts) {
        keybindings keys{};
        keys.reserve(texts.size());
        for (const std::string& text : texts) {
                keys.insert(QKeySequence{QString::fromStdString(text)}[0].key());
        }

        return keys;
}

std::vector<std::string> textFromTomlArray(const toml::array& arr) {
        std::vector<std::string> texts{};
        texts.reserve(arr.size());

        for (const auto& element : arr) {
                if (const auto& str_element = element.as_string()) {
                        texts.push_back(str_element->get());
                }
        }

        return texts;
}

/* Global Keys */
void KeysMapper::mapQuitKeys(NodePair nodes, keybindings& quit, const keybindings& defaults,
                             const PathContext& path_context) {
        toml::array keys{};
        Resolver::fromOrDefault<toml::array>({Source{.node  = nodes.primary,
                                                     .scope = applet::power_applet.scope},
                                              Source{.node  = nodes.fallback,
                                                     .scope = applet::global.scope}},
                                             keys, quit, defaults, path_context, {.min_size = 1},
                                             "Format: [keybindings...]");

        if (keys.empty()) { return; }

        quit = keysFromText(textFromTomlArray(keys));
}

/* Power Applet Keys*/
void KeysMapper::mapPrimaryButtonKeys(node_view node, std::vector<keybindings>& primary_buttons,
                                      const std::vector<keybindings>& defaults,
                                      const PathContext&              path_context) {
        toml::array keys{};
        Resolver::fromOrDefault({Source{.node = node, .scope = applet::power_applet.scope}}, keys,
                                primary_buttons, defaults, path_context, {.min_size = 1},
                                "Format: [keybindings...]");

        if (keys.empty()) { return; }

        std::vector<keybindings> found{};
        for (size_t i = 0; i != keys.size(); ++i) {
                keybindings found_for_button{};
                mapPrimaryButtonKey(node[i], found_for_button, defaults[i],
                                    path_context.getExtended(i));
                if (!keys.empty()) { found.push_back(found_for_button); }
        };

        primary_buttons = std::move(found);
}

void KeysMapper::mapPrimaryButtonKey(node_view node, keybindings& primary_button,
                                     const keybindings& defaults, const PathContext& path_context) {
        toml::array keys{};
        Resolver::fromOrDefault<toml::array>({Source{.node  = node,
                                                     .scope = applet::power_applet.scope}},
                                             keys, primary_button, defaults, path_context,
                                             {.min_size = 1}, "Format: [keybindings...]");

        if (keys.empty()) { return; }

        primary_button = keysFromText(textFromTomlArray(keys));
}
