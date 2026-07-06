// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ArgumentParser.h"
#include "Core/Applets/Types/AppletType.h"

#include <array>
#include <cstddef>
#include <format>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <QApplication>
#include <QDebug>
#include <QString>
#include <QStringView>
#include <QtGlobal>

void printHelpMenu(applet::type type) {
        QStringView applet_name = applet::toTitle(type);

        // clang-format off
        auto help_menu = QString{
                "Usage:\n"
                "\t%1\n"
                "Options:\n"
                "\t-c, --config <config path>\t\t\tConfig path\n"
                "\t-k, --keys <keys path>\t\t\t\tKeys path\n"
                "\t-h, -?, --help\t\t\t\t\tDisplay the help menu\n"
        }.arg(applet_name).toStdString();
        // clang-format on

        std::cout << help_menu;
}

void printArgs(int argc, char* argv[]) {
        QString formatted = {};

        for (int i = 0; i != argc; ++i) {
                formatted.append(QString("\n[%1]: [%2]").arg(i).arg(argv[i]));
        }

        qDebug() << "Arg count:" << argc;
        qDebug().noquote() << "Passed args:" << formatted;
}

bool ArgumentParser::isFlagName(std::string_view arg) {
        if (!arg.data() || arg.size() == 0) { qFatal("Received empty arg!"); }

        return arg[0] == '-';
}

bool ArgumentParser::isSingleFlag(std::string_view arg) {
        if (!arg.data() || arg.size() == 0) { qFatal("Received empty arg!"); }

        return arg == "-?" || arg == "-h" || arg == "--help";
}

void ArgumentParser::parseFlag(std::array<std::string_view, 2> flag, CmdArgs& parsed,
                               bool is_single_flag) {
        // Checks if both flags are valid before trying to dereference them.
        for (size_t i = 0; i != flag.size(); ++i) {
                if (!flag[i].data()) {
                        // It's expected for the second part of the flag to be missing if
                        // it's a single flag.
                        if (i == 1 && is_single_flag) { continue; }

                        qFatal("Passed flag part is null");
                }
        }

        if (flag[0] == "-c" || flag[0] == "--config") {
                parsed.config_path = QString::fromStdString(std::string(flag[1]));
        } else if (flag[0] == "-k" || flag[0] == "--keys") {
                parsed.keys_path = QString::fromStdString(std::string(flag[1]));
        } else if (flag[0] == "-?" || flag[0] == "-h" || flag[0] == "--help") {
                throw HelpMenuRequested();
        } else {
                throw HelpMenuRequested(std::format("Unrecognized flag {} {}", flag[0], flag[1]));
        }
}

CmdArgs ArgumentParser::parse(int argc, char* argv[]) {
        // Under normal circumstances, argc should always be at least 1, because program name is a
        // part of the argv array. Something must have gone wrong along the way if this executed.
        if (argc < 1) { qFatal("argc should always be at least 1, but here it's %i", argc); }

        CmdArgs flags = {};

        // Starts from 1 because argv[0] is the name of the program
        for (int i = 1; i != argc;) {
                std::array<std::string_view, 2> flag           = {};
                bool                            is_single_flag = false;

                std::string_view arg = argv[i] ? argv[i] : "NULL";

                if (isFlagName(argv[i])) {
                        if (isSingleFlag(argv[i])) {
                                flag           = {argv[i], {}};
                                is_single_flag = true;

                                // Moves beyond this argument
                                ++i;
                        } else if (i + 1 < argc) { // Pairs flag name and value if name is not last
                                flag = {argv[i], argv[i + 1]};

                                // Moves beyond this pair
                                i = i + 2;
                        } else { // Last argument but also a flag name - likely a stray flag name
                                throw HelpMenuRequested(std::format(
                                        "Stray flag name {} is missing an associated flag value",
                                        arg));
                        }
                } else { // Flag values should've been handled by the last if, this must be a stray
                        throw HelpMenuRequested(std::format(
                                "Stray flag value {} is not associated with any flag name", arg));
                }

                parseFlag(flag, flags, is_single_flag);
        }

        return std::move(flags);
}
