// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Core/Config/Resolve/Resolve.h"
#include "Core/Config/Types/ArraySpec.h"
#include "Core/Config/Types/Candidates/Candidates.h"
#include "Core/Config/Types/Keybindings.h"
#include "Core/Config/Types/NodeView.h"
#include "Core/UI/Types/PrimaryButtonBehavior.h"
#include "Core/UI/Types/PrimaryButtonStyle.h"
#include "Core/UI/Types/WindowParams.h"
#include "Map.h"

#include <cstddef>
#include <optional>
#include <string>
#include <toml++/toml.hpp>
#include <utility>
#include <vector>
#include <QApplication>
#include <QKeySequence>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <Qt>
#include <QtGlobal>

using namespace config;

/* Helpers */

std::optional<int> keyFromText(const std::string& text) {
        return QKeySequence(QString::fromStdString(text))[0].key();
}

std::optional<int> keyFromTomlElement(node_view element) {
        const auto* str = element.as_string();

        if (!str) { return std::nullopt; }

        return keyFromText(str->get());
}

keybindings keysFromTomlArray(const toml::array& arr) {
        keybindings keys = {};
        keys.reserve(arr.size());

        for (const auto& element : arr) {
                if (auto key = keyFromTomlElement(node_view(element))) { keys.insert(key.value()); }
        }

        return keys;
}

/* WindowParams */

WindowParams config::map::windowParams(const Candidates& candidates, const WindowParams& defaults) {
        return table<WindowParams>(candidates,
                                   [&defaults, &candidates](WindowParams& window) {
                                           window.size = resolve::from<QSize>(
                                                                 candidates.makeCopy().withExtension(
                                                                         u"size"))
                                                                 .value_or(defaults.size);

                                           window.title = resolve::from<QString>(
                                                                  candidates.makeCopy()
                                                                          .withExtension(u"title"))
                                                                  .value_or(defaults.title);
                                   })
                .value_or(defaults);
}

/* PrimaryButtonParams */

PrimaryButtonStyle config::map::primaryButtonStyle(const Candidates&         candidates,
                                                   const PrimaryButtonStyle& defaults) {
        return table<PrimaryButtonStyle>(
                       candidates,
                       [&defaults, &candidates](PrimaryButtonStyle& button) {
                               button.text_alignment = resolve::from<Qt::Alignment>(
                                                               candidates.makeCopy().withExtension(
                                                                       u"text_alignment"))
                                                               .value_or(defaults.text_alignment);

                               button.icon_alignment = resolve::from<Qt::Alignment>(
                                                               candidates.makeCopy().withExtension(
                                                                       u"icon_alignment"))
                                                               .value_or(defaults.icon_alignment);

                               button.icon_size = resolve::from<QSize>(
                                                          candidates.makeCopy().withExtension(
                                                                  u"icon_size"))
                                                          .value_or(defaults.icon_size);

                               button.policy = resolve::from<QSizePolicy>(
                                                       candidates.makeCopy().withExtension(
                                                               u"policy"))
                                                       .value_or(defaults.policy);
                       })
                .value_or(defaults);
}

PrimaryButtonBehavior config::map::primaryButtonBehavior(const Candidates&            candidates,
                                                         const PrimaryButtonBehavior& defaults) {
        return table<PrimaryButtonBehavior>(
                       candidates,
                       [&defaults, &candidates](PrimaryButtonBehavior& button) {
                               button.double_key_press =
                                       resolve::from<bool>(candidates.makeCopy().withExtension(
                                                                   u"double_key_press"))
                                               .value_or(defaults.double_key_press);
                       })
                .value_or(defaults);
}

/* KeysMapper */

keybindings config::map::quit(const Candidates& candidates, const keybindings& defaults) {
        const auto* keys = resolve::from<toml::array>(candidates,
                                                      {.bounds = ArrayBounds{.min_size = 1},
                                                       .format = u"[keybindings...]"});

        if (!keys || keys->empty()) { return defaults; }

        return keysFromTomlArray(*keys);
}

std::vector<keybindings> config::map::primaryButtons(const Candidates&               candidates,
                                                     const std::vector<keybindings>& defaults) {
        const auto* keys = resolve::from<toml::array>(candidates,
                                                      {.bounds = ArrayBounds{.min_size = 1},
                                                       .format = u"[keybindings...]"});

        if (!keys || keys->empty()) { return defaults; }

        std::vector<keybindings> buttons = {};
        buttons.reserve(keys->size());

        for (size_t i = 0; i != keys->size(); ++i) {
                keybindings found_for_button = primaryButton(candidates.makeCopy().withExtension(i),
                                                             defaults[i]);
                if (!found_for_button.empty()) { buttons.push_back(std::move(found_for_button)); }
        }

        return buttons;
}

keybindings config::map::primaryButton(const Candidates& candidates, const keybindings& defaults) {
        const auto* keys = resolve::from<toml::array>(candidates,
                                                      {.bounds = ArrayBounds{.min_size = 1},
                                                       .format = u"[keybindings...]"});

        if (!keys || keys->empty()) { return defaults; }

        return keysFromTomlArray(*keys);
}
