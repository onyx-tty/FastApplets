// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Core/Args/Args.h"

#include <QDebug>
#include <QObject>
#include <QTest>
#include <QtGlobal>
#include <array>
#include <cstddef>
#include <format>
#include <string_view>
#include <tuple>

class ArgsTest final : public QObject {
        Q_OBJECT

private slots:
        // TODO: In IsFlagName and IsSingle Flag, test for null and empty flags.
        //       This is not possible right now as the functions qFatal if they
        //       encounter that, and don't throw anything.
        static void isFlagName_returnsTrueForFlagNames() {
                for (const auto* flag : {"--name", "-h"}) {
                        QVERIFY2(arg::isFlagName(flag),
                                std::format("{} must be recognized as a flag name", flag).c_str());
                }
        }

        static void isFlagName_returnsFalseForNonFlagNames() {
                QVERIFY2(!arg::isFlagName("abc"), "abc must not be recognized as a flag name");
        }

        static void isSingleFlag_returnsTrueForHelp() {
                for (const auto* flag : {"-?", "-h", "--help"}) {
                        QVERIFY2(arg::isSingleFlag(flag),
                                std::format("{} must be recognized as a single flag", flag).c_str());
                }
        }

        static void isSingleFlag_returnsFalseForUnlistedFlags() {
                QVERIFY2(!arg::isSingleFlag("--flag-doesnt-exist"),
                        "--flag-doesnt-exist should not be recognized as a single flag");
                QVERIFY2(!arg::isSingleFlag("-c"),
                        "-c is a config flag, it must not be recognized as a single flag");
        }

        static void parseDoubleFlag_handlesSupportedFlags() {
                arg::CmdArgs args = {};

                // The args member is specified beforehand, because it's clear that the config
                // args will go to .config_path, and the keys one to .keys_path.
                using TestCases =
                        std::array<std::tuple<std::string_view, std::string_view, QString&>, 4>;
                const TestCases test_cases = {
                        {{"-c", "/config/path", args.config_path},
                         {"--config", "/config/path", args.config_path},
                         {"-k", "/keys/path", args.keys_path},
                         {"--keys", "/keys/path", args.keys_path}}
                };

                for (const auto& [key, value, member] : test_cases) {
                        arg::parseDoubleFlag({key, value}, args);
                        // TODO: If member != value, then just in case check if the other remaining
                        //       members are storing anything. That way if it's a problem caused
                        //       by parseFlag() assigning to the wrong member, a message noting
                        //       it will help narrow the problem.
                        QVERIFY2(member.toStdString() == value,
                                std::format("{} must be recognized", key).c_str());
                        args = {};
                }
        }

        static void parseDoubleFlag_throwsIfFlagsInvalid() {
                using TestCases = std::array<std::tuple<std::string_view, std::string_view, int>, 2>;
                constexpr TestCases test_cases = {
                        {{"key", "", 0}, {"", "value", 1}}
                };

                for (auto [key, value, index] : test_cases) {
                        bool is_invalid = false;

                        try {
                                arg::CmdArgs args = {};
                                arg::parseDoubleFlag({key, value}, args);
                        } catch (const HelpMenuRequested&) { is_invalid = true; }

                        QVERIFY2(is_invalid,
                                std::format("Invalid flag[{}] must throw HelpMenuRequested", index)
                                        .c_str());
                }
        }

        static void parseDoubleFlag_throwsIfUnrecognized() {
                bool is_unrecognized = false;

                try {
                        arg::CmdArgs args = {};
                        arg::parseDoubleFlag({"-unrecognized", "-flag"}, args);
                } catch (const HelpMenuRequested&) { is_unrecognized = true; }

                // clang-format off
                QVERIFY2(is_unrecognized,
                        "Unrecognized arguments must throw,"
                        "-not-a-valid-argument was somehow recognized");
                // clang-format on
        }

        static void parseSingleFlag_handlesSupportedFlags() {
                for (const auto* help_flags : {"-?", "-h", "--help"}) {
                        try {
                                arg::parseSingleFlag(help_flags);
                        } catch (const HelpMenuRequested&) {
                                QVERIFY(true);
                                continue;
                        }

                        QVERIFY2(false, "All help flags must be recognized");
                }
        }

        static void parseSingleFlag_throwsIfHelpPassed() {
                for (const auto* flag : {"-?", "-h", "--help"}) {
                        bool has_thrown = false;

                        try {
                                arg::parseSingleFlag(flag);
                        } catch (const HelpMenuRequested&) { has_thrown = true; }

                        QVERIFY2(has_thrown,
                                std::format("{} must throw HelpMenuRequested", flag).c_str());
                }
        }

        static void parseSingleFlag_throwsIfFlagInvalid() {
                bool is_invalid = false;

                try {
                        arg::parseSingleFlag("");
                } catch (const HelpMenuRequested&) { is_invalid = true; }

                QVERIFY2(is_invalid, "Invalid single flag must throw HelpMenuRequested");
        }

        static void parseSingleFlag_throwsIfUnrecognized() {
                bool has_thrown = false;

                try {
                        arg::parseSingleFlag("-not-a-valid-argument");
                } catch (const HelpMenuRequested&) { has_thrown = true; }

                // clang-format off
                QVERIFY2(has_thrown,
                        "Unrecognized arguments must throw,"
                        "-not-a-valid-argument was somehow recognized");
                // clang-format on
        }

        static void parse_handlesSingleFlags() {
                bool handled_single_flag = false;

                constexpr const char* argv[] = {"AppletName", "--help"};

                // The only usable single flag at the moment is the help flag
                // so only that one is tested
                try {
                        arg::CmdArgs _ = arg::parse(2, argv);
                } catch (const HelpMenuRequested&) { handled_single_flag = true; }

                QVERIFY2(handled_single_flag, "Must handle single flags");
        }

        static void parse_handlesDoubleFlags() {
                constexpr const char* argv[] = {"AppletName", "-c", "/config/path", "-k",
                        "/keys/path"};

                arg::CmdArgs args = arg::parse(5, argv);

                const auto parsed_everything = [&args]() -> bool {
                        return args.config_path == "/config/path" && args.keys_path == "/keys/path";
                };

                QVERIFY2(parsed_everything(), "Must parse double flags");
        }

        static void parse_throwsOnStrayFlagName() {
                constexpr bool is_single_flag = false;
                bool           has_thrown     = false;

                size_t                argc   = 2;
                constexpr const char* argv[] = {"FastApplet", "--stray-name"};

                try {
                        auto _ = arg::parse(2, argv);
                } catch (const HelpMenuRequested&) { has_thrown = true; }

                QVERIFY2(has_thrown, "Must throw on stray flag names");
        }

        static void parse_throwsOnStrayFlagValue() {
                constexpr bool is_single_flag = false;
                bool           has_thrown     = false;

                constexpr const char* argv[] = {"FastApplet", "stray_value"};

                try {
                        auto _ = arg::parse(2, argv);
                } catch (const HelpMenuRequested&) { has_thrown = true; }

                QVERIFY2(has_thrown, "Must throw on stray flag values");
        }
};

QTEST_MAIN(ArgsTest);
#include "ArgsTest.moc"
