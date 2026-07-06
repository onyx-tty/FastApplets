// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/AppletType.h"

#include <array>
#include <string_view>
#include <QString>

void printHelpMenu(applet::type type);
void printArgs(int argc, char* argv[]);

struct CmdArgs {
        QString config_path;
        QString keys_path;
};

class ArgumentParser final {
private:
        static bool isFlagName(std::string_view arg);
        static bool isSingleFlag(std::string_view arg);
        static void parseFlag(std::array<std::string_view, 2> flag, CmdArgs& parsed,
                              applet::type type, bool is_single_flag = false);

public:
        ArgumentParser() = delete;
        static CmdArgs parse(int argc, char* argv[], applet::type type);
};
