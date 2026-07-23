// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Core/Args/Args.h"

#include <array>
#include <cstddef>
#include <format>
#include <functional>
#include <string_view>
#include <tuple>
#include <QDebug>
#include <QObject>
#include <QTest>
#include <QtGlobal>

namespace {

// TODO: This function is overcomplicated
// TODO: Single flag should be a separate overload, both in arg:: and here
[[nodiscard]] bool parseFlag_hasThrownHelp(std::array<std::string_view, 2> flag,
                                           bool                            is_single_flag) {
        // Flags must be valid before dereferencing
        for (size_t i = 0; i != flag.size(); ++i) {
                if (!flag[i].data()) {
                        // It's expected for the second part of the flag to be missing if
                        // it's a single flag.
                        if (i == 1 && is_single_flag) { continue; }

                        qFatal("Passed null flag[0]");
                }
        }

        try {
                arg::CmdArgs args = {};
                arg::parseFlag(flag, args, is_single_flag);
        } catch (const HelpMenuRequested& e) { return true; }

        return false;
}

[[nodiscard]] bool parseFlag_hasThrownHelp(std::string_view single_flag) {
        constexpr bool is_single_flag = true;
        return parseFlag_hasThrownHelp({single_flag, {}}, is_single_flag);
};

} // namespace

class ArgsTest final : public QObject {
        Q_OBJECT

private slots:
        // TODO: In IsFlagName and IsSingle Flag, test for null and empty flags.
        //       This is not possible right now as the functions qFatal if they
        //       encounter that, and don't throw anything.
        static void isFlagName_returnsTrueForFlagNames() {
                for (const auto* flag : {"--name", "-h"}) {
                        QVERIFY2(arg::isFlagName(flag),
                                 std::format("{} must be recognized as a flag name!", flag).c_str());
                }
        }

        static void isFlagName_returnsFalseForNonFlagNames() {
                QVERIFY2(!arg::isFlagName("abc"), "abc must not be recognized as a flag name!");
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

        static void parseFlag_handlesSupportedFlags() {
                constexpr bool is_single_flag = false;
                arg::CmdArgs   args           = {};

                // The args member is specified beforehand, because it's clear that the config
                // args will go to .config_path, and the keys one to .keys_path.
                const auto test_cases =
                        {std::tuple("-c", "/config/path", std::ref(args.config_path)),
                         std::tuple("--config", "/config/path", std::ref(args.config_path)),
                         std::tuple("-k", "/keys/path", std::ref(args.keys_path)),
                         std::tuple("--keys", "/keys/path", std::ref(args.keys_path))};

                for (const auto& [key, value, member] : test_cases) {
                        arg::parseFlag({key, value}, args, is_single_flag);
                        // TODO: If member != value, then just in case check if the other remaining
                        //       members are storing anything. That way if it's a problem caused
                        //       by parseFlag() assigning to the wrong member, a message noting
                        //       it will help narrow the problem.
                        QVERIFY2(member == value, std::format("{} must be recognized", key).c_str());
                        args = {};
                }
        }

        static void parseFlag_throwsIfHelpPassed() {
                for (const auto* flag : {"-?", "-h", "--help"}) {
                        QVERIFY2(parseFlag_hasThrownHelp(flag),
                                 std::format("{} must throw HelpMenuRequested", flag).c_str());
                }
        }

        static void parseFlag_throwsIfUnrecognized() {
                // clang-format off
                QVERIFY2(parseFlag_hasThrownHelp("-not-a-valid-argument"),
                        "Unrecognized arguments must throw,"
                        "-not-a-valid-argument was somehow recognized");
                // clang-format on
        }

        static void parse_handlesSingleFlags() {
                bool handled_single_flag = false;

                constexpr const char* argv[] = {"AppletName", "--help"};

                // The only usable single flag at the moment is the help flag
                // so it's the only one
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
                constexpr bool is_single_flag  = false;
                bool           has_thrown_help = false;

                size_t                argc   = 2;
                constexpr const char* argv[] = {"FastApplet", "--stray-name"};

                try {
                        auto _ = arg::parse(2, argv);
                } catch (const HelpMenuRequested&) { has_thrown_help = true; }

                QVERIFY2(has_thrown_help, "Must throw on stray flag names");
        }

        static void parse_throwsOnStrayFlagValue() {
                constexpr bool is_single_flag  = false;
                bool           has_thrown_help = false;

                constexpr const char* argv[] = {"FastApplet", "stray_value"};

                try {
                        auto _ = arg::parse(2, argv);
                } catch (const HelpMenuRequested&) { has_thrown_help = true; }

                QVERIFY2(has_thrown_help, "Must throw on stray flag values");
        }
};

QTEST_MAIN(ArgsTest);
#include "ArgsTest.moc"
