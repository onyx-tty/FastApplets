// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/Types/NodeView.h"

#include <QString>
#include <TomlQt/ArrayBounds.h>
#include <array>
#include <optional>
#include <string_view>
#include <type_traits>

namespace config {

using tomlqt::ArrayBounds;

// Class that stores sources of evaluation, the path corresponding to them, and the filename
// of the file they belong to.
//
// It has three responsibilities:
// - Store context needed for the resolution of TOML nodes, this includes sources, path,
//   and filename.
// - Extend the class and its members in place, using the subscript operator.
// - Finally, resolve the sources stored inside, while logging the results using path and filename.
class ConfigView final {
private:
        std::array<node_view, 2> sources;
        QString                  path;
        QString                  filename;

public:
        ConfigView(node_view applet, node_view global, QString path, QString filename);

        // Returns a ConfigView copy with sources (nodes) and path extended by string_view key.
        //
        // As a result:
        // source[0] -> source[0][key]
        // source[1] -> source[1][key]
        // path -> path + key (additionally, preceded by '.' if not empty)
        // filename stays the same
        ConfigView operator[](std::string_view key) const;

        // Returns a ConfigView copy with sources (nodes) and path extended by string_view key.
        //
        // As a result:
        // source[0] -> source[0][index]
        // source[1] -> source[1][index]
        // path -> path[index]
        // filename stays the same
        ConfigView operator[](int index) const;

        // Returns std::optional<T> where T is the wrapped, expected type.
        //
        // If a problem is encountered, an error message is logged and the function returns
        // std::nullopt.
        //
        // On success, valid non-nullopt std::optional<T> is returned.
        template<typename T>
        requires(!std::is_same_v<T, toml::array>)
        std::optional<T> resolve() const;

        // Returns const toml::array* while optionally checking for bounds.
        //
        // If a problem is encountered, an error message is logged and the function returns
        // nullptr.
        //
        // On success, valid non-null const toml::array* is returned.
        template<typename T>
        requires(std::is_same_v<T, toml::array>)
        const toml::array* resolve(const ArrayBounds& array_bounds = {}) const;
};

} // namespace config

#include "View.tpp"
