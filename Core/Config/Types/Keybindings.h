// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QDebug>
#include <Qt>
#include <QtGlobal>
#include <unordered_set>
#include <vector>

using keybindings = std::unordered_set<int>;

// Generates a vector of keybindings in range between 'first' and 'last', inclusive.
// Example: makeKeyRange(Qt::Key_1, Qt::Key_4) -> {Qt::Key_1, Qt::Key_2, Qt::Key_3, Qt::Key_4}
[[nodiscard]] inline std::vector<keybindings> makeKeyRange(Qt::Key first, Qt::Key last) {
        if (first > last) { qFatal("First must come before last"); }

        std::vector<keybindings> ret = {};
        ret.reserve(last - first + 1);

        for (int key = first; key <= last; ++key) { ret.push_back(keybindings(key)); }

        return ret;
}
