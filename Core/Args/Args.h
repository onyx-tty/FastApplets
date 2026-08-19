// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/Type.h"
#include "Core/Config/Types/Filepaths.h" // Only needed for injectArgs

#include <QString>
#include <array>
#include <stdexcept>
#include <string>
#include <string_view>

void printHelpMenu(applet::Type type);
void printArgs(int argc, const char* const argv[]);

// Expects the program to print out the help menu and terminate right away.
class HelpMenuRequested final : public std::runtime_error {
public:
        explicit HelpMenuRequested(const std::string& reason = "") : std::runtime_error(reason) {};
};

namespace arg {

// Stores a list of parsed command-line arguments.
struct CmdArgs {
        QString config_path;
        QString keys_path;
};

// Checks if passed arg starts with '-'.
//
// Terminates with qFatal if arg is invalid or is just one character.
[[nodiscard]] bool isFlagName(std::string_view arg);

// Checks if passed args qualifies as a single flag.
// Currently only 'help'.
//
// Terminates with qFatal if arg is invalid or is just one character.
[[nodiscard]] bool isSingleFlag(std::string_view arg);

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
void parseFlag(std::array<std::string_view, 2> flag, CmdArgs& parsed, bool is_single_flag = false);

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
// TODO: Locator should be used to try to find the config/keys file if passed.
//       If the path is invalid, the program should warn the user and throw HelpMenuRequested.
[[nodiscard]] CmdArgs parse(int argc, const char* const argv[]);

// Inject config filepath and keys filepath if they are valid
// TODO: This is a workaround. Ideally, valid args should be assigned prior to config::locateFiles
//       lookups. Currently that's not possible without collapsing config::locateFiles
//       and separating the lookups of config and keys; paths for each have to be injected
//       in separation for that to work
void injectArgs(arg::CmdArgs& args, config::Filepaths& filepaths);

} // namespace arg
