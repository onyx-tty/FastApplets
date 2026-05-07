// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

class QString;

class ShellRunner final {
public:
        ShellRunner() = delete;

        // Single quotes are treated as double quotes
        //
        // Example: "hyprshutdown -t 'Shutting down...' -p 'systemctl poweroff'"
        // "hyprshutdown" is treated as the program and the rest as arguments
        static void runCommand(QString command);
};
