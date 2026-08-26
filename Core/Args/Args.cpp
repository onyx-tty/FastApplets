// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Args.h"
#include "Core/Applets/Types/Type.h"
#include "Core/Config/Types/Filepaths.h" // Only needed for injectArgs

#include <QApplication>
#include <QDebug>
#include <QFileInfo>
#include <QString>
#include <QStringView>
#include <QtGlobal>
#include <array>
#include <cstddef>
#include <format>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>

void printHelpMenu(applet::Type type) {
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

void printArgs(int argc, const char* const argv[]) {
        QString formatted = {};

        for (int i = 0; i != argc; ++i) {
                formatted.append(QString("\n[%1]: [%2]").arg(i).arg(argv[i]));
        }

        qDebug() << "Arg count:" << argc;
        qDebug().noquote() << "Passed args:" << formatted;
}

bool arg::isFlagName(std::string_view arg) {
        if (!arg.data() || arg.size() == 0) { qFatal("Received empty arg!"); }

        return arg[0] == '-';
}

bool arg::isSingleFlag(std::string_view arg) {
        if (!arg.data() || arg.size() == 0) { qFatal("Received empty arg!"); }

        return arg == "-?" || arg == "-h" || arg == "--help";
}

void arg::parseDoubleFlag(std::array<std::string_view, 2> flag, arg::CmdArgs& parsed) {
        // Checks if both flags are valid before trying to dereference them.
        if (!flag[0].data() || flag[0].empty()) { qFatal("Passed null flag[0]"); }
        if (!flag[1].data() || flag[1].empty()) { parseSingleFlag(flag[0]); }

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

void arg::parseSingleFlag(std::string_view flag) {
        // Checks if both flags are valid before trying to dereference them.
        if (!flag.data() || flag.empty()) {
                throw HelpMenuRequested(std::format("Passed flag is null"));
        }

        if (flag == "-?" || flag == "-h" || flag == "--help") {
                throw HelpMenuRequested();
        } else {
                throw HelpMenuRequested(std::format("Unrecognized flag {}", flag));
        }
}

arg::CmdArgs arg::parse(int argc, const char* const argv[]) {
        // Under normal circumstances, argc should always be at least 1, because program name is a
        // part of the argv array. Something must have gone wrong along the way if this executed.
        if (argc < 1) { qFatal("argc should always be at least 1, but here it's %i", argc); }

        arg::CmdArgs flags = {};

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

                if (is_single_flag) {
                        parseSingleFlag(flag[0]);
                } else {
                        parseDoubleFlag(flag, flags);
                }
        }

        return std::move(flags);
}

void arg::injectArgs(arg::CmdArgs& args, config::Filepaths& filepaths) {
        const auto injector = [](QString& target, QString& source, QStringView source_name) {
                QFileInfo file = QFileInfo(source);
                if (file.exists() && file.isFile() && file.isReadable()) {
                        qDebug() << QString(
                                "Found file %1, it was passed as a %2 cmd argument, injecting")
                                            .arg(source)
                                            .arg(source_name);
                        target = source;
                } else if (!source.isEmpty()) {
                        qWarning()
                                << QString("File %1 not found, it was passed as a %2 cmd argument")
                                           .arg(source)
                                           .arg(source_name);
                }
        };

        injector(filepaths.config, args.config_path, u"config");
        injector(filepaths.keys, args.keys_path, u"keys");
}
