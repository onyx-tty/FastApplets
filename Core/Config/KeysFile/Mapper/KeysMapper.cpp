// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "KeysMapper.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"
#include "Core/Config/Resolver/PathContext/PathContext.h"
#include "Core/Config/Resolver/Resolver.h"
#include "Core/Config/Resolver/Types/ResolverCandidate.h"

#include <cstddef>
#include <string>
#include <toml++/toml.hpp>
#include <utility>
#include <vector>
#include <QApplication>
#include <QKeySequence>
#include <QString>

keybindings keysFromText(const std::vector<std::string>& texts) {
        keybindings keys = {};
        keys.reserve(texts.size());

        for (const std::string& text : texts) {
                keys.insert(QKeySequence{QString::fromStdString(text)}[0].key());
        }

        return keys;
}

std::vector<std::string> textFromTomlArray(const toml::array& arr) {
        std::vector<std::string> texts = {};
        texts.reserve(arr.size());

        for (const auto& element : arr) {
                if (const auto& str_element = element.as_string()) {
                        texts.push_back(str_element->get());
                }
        }

        return texts;
}

/* Global Keys */
keybindings KeysMapper::quit(const ResolverCandidates& candidates, const keybindings& defaults,
                             const PathContext& path_context) {
        toml::array keys = Resolver::from<toml::array>(candidates, path_context, {.min_size = 1},
                                                       u"[keybindings...]")
                                   .value_or(toml::array());

        if (keys.empty()) { return defaults; }

        return keysFromText(textFromTomlArray(keys));
}

/* Primary Applet Keys*/
std::vector<keybindings> KeysMapper::primaryButtons(const ResolverCandidates&       candidates,
                                                    const std::vector<keybindings>& defaults,
                                                    const PathContext&              path_context) {
        toml::array keys = Resolver::from<toml::array>(candidates, path_context, {.min_size = 1},
                                                       u"[keybindings...]")
                                   .value_or(toml::array());

        if (keys.empty()) { return defaults; }

        std::vector<keybindings> buttons = {};
        buttons.reserve(keys.size());
        for (size_t i = 0; i != keys.size(); ++i) {
                keybindings found_for_button = primaryButton(candidates.makeExtended(i),
                                                             defaults[i],
                                                             path_context.makeExtended(i));
                if (!keys.empty()) { buttons.push_back(std::move(found_for_button)); }
        }

        return std::move(buttons);
}

keybindings KeysMapper::primaryButton(const ResolverCandidates& candidates,
                                      const keybindings&        defaults,
                                      const PathContext&        path_context) {
        toml::array keys = Resolver::from<toml::array>(candidates, path_context, {.min_size = 1},
                                                       u"[keybindings...]")
                                   .value_or(toml::array());

        if (keys.empty()) { return defaults; }

        return keysFromText(textFromTomlArray(keys));
}
