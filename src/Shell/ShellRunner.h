// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

class QString;

// Handles shell command execution (fire-and-forget, no output capture)
class ShellRunner final {
public:
        ShellRunner() = delete;

        // Run a string shell command
        //
        // QProcess::splitCommand() does not interpret single quotes as quotes,
        // but as regular characters. This function converts single quotes to
        // double quotes before parsing, to work around that limitation.
        //
        // TODO: Quote conversion should happen at config parse, not here
        //
        // Example: "hyprshutdown -t 'Shutting down...' -p 'systemctl poweroff'"
        //   "hyprshutdown" = program, rest = arguments
        static void command(QString command);
};
