// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/AppletType.h"

#include <array>
#include <stdexcept>
#include <string>
#include <string_view>
#include <QString>

void printHelpMenu(applet::type type);
void printArgs(int argc, char* argv[]);

// Stores a list of parsed command-line arguments.
// TODO: Move to Args/Types.
struct CmdArgs {
        QString config_path;
        QString keys_path;
};

// Expects the program to print out the help menu and terminate right away.
class HelpMenuRequested final : public std::runtime_error {
public:
        explicit HelpMenuRequested(const std::string& reason = "") : std::runtime_error(reason) {};
};

// Manages parsing of passed raw command-line arguments.
class ArgumentParser final {
private:
        // Checks if passed arg starts with '-'.
        //
        // Terminates with qFatal if arg is invalid or is just one character.
        static bool isFlagName(std::string_view arg);

        // Checks if passed args qualifies as a single flag.
        // Currently only 'help'.
        //
        // Terminates with qFatal if arg is invalid or is just one character.
        static bool isSingleFlag(std::string_view arg);

        // Parses a 'flag' by finding flag[0] on the list and the assigning flag[1] to 'parsed'.
        // Exceptions to that rule:
        // - Passed -?/-h/--help, which throws an empty HelpMenuRequested instead.
        // - Passed an unrecognized flag, which throws a non-empty HelpMenuRequested instead.
        //
        // Terminates with qFatal if the flag fails a null check for either part.
        // The only exception is flag[1] if is_single_flag = true, because that means
        // only flag[0] will be used.
        // TODO: Create an overload for single flags instead.
        //
        // Currently supports config, keys, and help.
        static void parseFlag(std::array<std::string_view, 2> flag, CmdArgs& parsed,
                              applet::type type, bool is_single_flag = false);

public:
        ArgumentParser() = delete;

        // Parses raw argc and argv into CmdArgs.
        //
        // Throws empty HelpMenuRequested if -?/-h/--help was explicitly called as a flag.
        //
        // Additionally, throws HelpMenuRequested with a non-empty reason if found:
        // - A stray flag name without an associated value.
        // - A stray flag value without an associated name.
        // - An unrecognized flag name.
        //
        // Calls qFatal if argc is less than 1, indicating corruption.
        static CmdArgs parse(int argc, char* argv[], applet::type type);
};
