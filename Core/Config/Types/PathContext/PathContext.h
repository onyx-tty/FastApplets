// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/Type.h"

#include <QString>
#include <QStringView>
#include <cstddef>

namespace config {

// Builds logging paths with dynamic applet name substitution.
//
// PathContext stores a filename and a navigation path (e.g. ".window.size").
// When logging messages, the name ("power_applet", "global", ...) is inserted
// at runtime to produce full paths like:
//   "in config.toml, power_applet.window.size"
//   "in config.toml, global.window.size"
//
// With this it's not necessary to store duplicate paths for each name in Candidates.
class PathContext final {
private:
        QString filename;
        QString path_context;
        char    separator;

public:
        // Creates a context for path construction.
        //
        // Example: PathContext(u"config.toml", u".window")
        explicit PathContext(QStringView filename, QStringView path_context, char separator = '.');

        // Creates a copy of PathContext. An alternative to PathContext(old)
        // that makes the intention clearer when chaining.
        [[nodiscard]] PathContext makeCopy() const;

        // Extends PathContext by an additional path segment.
        //
        // Example: "window" -> context.withExtension(u"size") -> ".window.size"
        PathContext& withExtension(QStringView segment);

        // Extends PathContext by an array index.
        //
        // Index is formatted with brackets, no separator is added.
        //
        // Example: ".window.size" context.withExtension(2) -> ".window.size[2]"
        PathContext& withExtension(size_t index);

        // Returns a full logging path by inserting name between filename and
        // path_context.
        //
        // Example: makePath("power_applet") -> "in config.toml, power_applet.window"
        [[nodiscard]] QString makePath(applet::Type applet) const;
};

} // namespace config
