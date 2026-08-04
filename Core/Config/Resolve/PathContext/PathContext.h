// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/Type.h"

#include <cstddef>
#include <string_view>
#include <QString>
#include <QStringView>

namespace config::resolve {

// Builds logging paths with dynamic applet name substitution.
//
// PathContext stores a filename and a navigation path (e.g. ".window.size").
// When logging messages, the name ("power_applet", "global", ...) is inserted
// at runtime to produce full paths like:
//   "in config.toml, power_applet.window.size"
//   "in config.toml, global.window.size"
//
// With this it's not necessary to store duplicate paths for each name in NodePair.
class PathContext final {
private:
        QString path_context;
        QString filename;
        char    separator;

public:
        // Creates a context for path construction.
        //
        // Example: PathContext{u"config.toml", u".window"}
        explicit PathContext(QStringView filename, QStringView path_context, char separator = '.');

        // Creates a copy of PathContext. An alternative to PathContext(old)
        // that makes the intention clearer when chaining.
        [[nodiscard]] PathContext makeCopy() const;

        // Extends PathContext by an additional path segment.
        //
        // Example: "window" -> context.withExtension("size") -> ".window.size"
        [[nodiscard]] PathContext& withExtension(std::string_view segment);

        // Extends PathContext by an array index.
        //
        // Index is formatted with brackets, no separator is added.
        //
        // Example: ".window.size" context.withExtension(2) -> ".window.size[2]"
        [[nodiscard]] PathContext& withExtension(size_t index);

        // Returns a full logging path by inserting name between filename and
        // path_context.
        //
        // Example: makePath("power_applet") -> "in config.toml, power_applet.window"
        [[nodiscard]] QString makePath(applet::type applet) const;
};

} // namespace config::resolve
