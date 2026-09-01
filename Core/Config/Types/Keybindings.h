// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QDebug>
#include <Qt>
#include <QtGlobal>
#include <initializer_list>
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

// Assigns unclaimed incoming_keys to target and adds them to claimed_keys to guard against
// future reuse. If an incoming key is already claimed, the function logs that and ignores
// that key.
inline void claimKeys(
        keybindings& target, keybindings& claimed_keys, std::initializer_list<int> incoming_keys) {
        if (&target == &claimed_keys) {
                qWarning() << "Passed the same key container as both target and claimed_keys."
                           << "Ensure this is intended";
        }

        for (auto key : incoming_keys) {
                if (claimed_keys.contains(key)) {
                        qInfo() << "Key" << key << "is already claimed";
                        continue;
                }

                target.insert(key);
                claimed_keys.insert(key);
        }
}
