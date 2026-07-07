// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

class QString;

// Handles shell command execution (no output capture).
class ShellRunner final {
public:
        ShellRunner() = delete;

        // Runs a string shell command.
        // It immediately returns and warns if passed command is empty, to avoid errors.
        // The command is executed by passing it to '/bin/sh -c'.
        //
        // Example: "hyprshutdown -t 'Shutting down...' -p 'systemctl poweroff'"
        static void command(QString command);
};
