// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Applets/Types/AppletRecord.h"
#include "Applets/Types/AppletType.h"

#include <cstddef>
#include <string_view>
#include <QString>

class QStringView;

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

        // Returns a full logging path by inserting name between filename and
        // path_context.
        //
        // Example: makePath("power_applet") -> "in config.toml, power_applet.window"
        QString makePath(applet::type applet) const;

        // Returns a new PathContext with an additional path segment appended.
        //
        // Example: context.getExtended("size") -> PathContext with ".window.size"
        PathContext getExtended(std::string_view segment) const;

        // Returns a new PathContext with an array index appended.
        //
        // Index is formatted with brackets, no separator is added.
        //
        // Example: ctx.getExtended(2) -> PathContext with ".window.size[2]"
        PathContext getExtended(size_t index) const;
};
