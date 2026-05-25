// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "KeysMapper.h"
#include "Applets/Types/AppletRecord.h"
#include "Config/KeysFile/Types/Keybindings.h"
#include "Config/Resolver/PathContext/PathContext.h"
#include "Config/Resolver/Resolver.h"
#include "Config/Resolver/Types/ResolverCandidate.h"
#include "Config/Types/NodeView.h"

#include <cstddef>
#include <initializer_list>
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
keybindings KeysMapper::quit(std::initializer_list<ResolverCandidate> candidates,
                             const keybindings& defaults, const PathContext& path_context) {
        toml::array keys{};
        keybindings quit{};
        Resolver::fromOrDefault<toml::array>(candidates, keys, quit, defaults, path_context,
                                             {.min_size = 1}, "Format: [keybindings...]");

        if (keys.empty()) { return defaults; }

        return keysFromText(textFromTomlArray(keys));
}

/* Power Applet Keys*/
std::vector<keybindings> KeysMapper::primaryButtons(node_view                       node,
                                                    const std::vector<keybindings>& defaults,
                                                    const PathContext&              path_context) {
        toml::array              keys{};
        std::vector<keybindings> primary_buttons{};

        Resolver::fromOrDefault({ResolverCandidate{.node   = node,
                                                   .applet = applet::power_applet.type}},
                                keys, primary_buttons, defaults, path_context, {.min_size = 1},
                                "Format: [keybindings...]");

        if (keys.empty()) { return defaults; }

        std::vector<keybindings> found{};
        for (size_t i = 0; i != keys.size(); ++i) {
                keybindings found_for_button = primaryButton(node[i], defaults[i],
                                                             path_context.getExtended(i));
                if (!keys.empty()) { found.push_back(std::move(found_for_button)); }
        };

        return std::move(found);
}

keybindings KeysMapper::primaryButton(node_view node, const keybindings& defaults,
                                      const PathContext& path_context) {
        toml::array keys{};
        keybindings primary_button{};

        Resolver::fromOrDefault<toml::array>({ResolverCandidate{.node = node,
                                                                .applet = applet::power_applet.type}},
                                             keys, primary_button, defaults, path_context,
                                             {.min_size = 1}, "Format: [keybindings...]");

        if (keys.empty()) { return defaults; }

        return keysFromText(textFromTomlArray(keys));
}
