// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Core/Config/TomlParser/TomlParser.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QLatin1StringView>
#include <QObject>
#include <QTest>
#include <QTextStream>

using namespace Qt::Literals::StringLiterals;

class TomlParserTest final : public QObject {
        Q_OBJECT

        static constexpr QLatin1StringView test_dirpath          = "/tmp/FastAppletsTests"_L1;
        static constexpr QLatin1StringView test_filename_valid   = "TomlParserTestOK.toml"_L1;
        static constexpr QLatin1StringView test_filename_invalid = "TomlParserTestBAD.toml"_L1;
        static constexpr const char*       test_contents_valid   = R"(
                [table]
                name = "FastApplets"
                year = 2026
                status = true
                applets = ["PowerApplet"]
                half = 0.5
                )";
        static constexpr const char*       test_contents_invalid = R"(
                [wrong syntax
                abc = 5
                = 1)";

private slots:
        static void initTestCase() {
                // Verify that /tmp and test directory are accessible
                // before writing anything
                QDir test_parentdir("/tmp");
                QVERIFY2(test_parentdir.exists() && test_parentdir.isReadable()
                                 && QFileInfo(test_parentdir.path()).isWritable(),
                         "No access to /tmp");

                QVERIFY2(test_parentdir.mkdir("FastAppletsTests"),
                         "Failed to create test directory");

                // Write minimal valid and invalid TOML files for parser tests
                QFile valid(QDir(test_dirpath).filePath(test_filename_valid));
                QVERIFY2(valid.open(QIODevice::WriteOnly | QIODevice::Text),
                         "Failed to create test valid TOML file");

                // Valid
                QTextStream writer(&valid);
                writer << test_contents_valid;
                valid.close();
                writer.flush();

                // Invalid
                QFile invalid(QDir(test_dirpath).filePath(test_filename_invalid));
                QVERIFY2(invalid.open(QIODevice::WriteOnly | QIODevice::Text),
                         "Failed to create test invalid TOML file");
                writer.setDevice(&invalid);
                writer << test_contents_invalid;
                invalid.close();
        }

        static void cleanupTestCase() {
                // Double-check to guard against path corruption
                QVERIFY2(test_dirpath.startsWith("/tmp/"_L1), "Test directory must be under /tmp/");

                QDir test_dir(test_dirpath);
                if (test_dir.exists()) {
                        QVERIFY2(test_dir.removeRecursively(), "Failed to remove test directory");
                }
        }

        static void handlesEmptyFilepath() {
                QVERIFY2(TomlParser::file("").empty(),
                         "Empty filepath must return empty toml::table");
        }

        static void handlesMissingFile() {
                QVERIFY2(TomlParser::file("/tmp/invalid/path/found.toml").empty(),
                         "Missing file must return empty toml::table");
        }

        static void handlesInvalidToml() {
                const auto invalid = QDir(test_dirpath).filePath(test_filename_invalid);

                QVERIFY2(TomlParser::file(invalid).empty(),
                         "File with invalid TOML must return empty toml::table");
        }

        static void handlesValidToml() {
                const auto valid = QDir(test_dirpath).filePath(test_filename_valid);

                QVERIFY2(!TomlParser::file(valid).empty(),
                         "Failed to parse file with valid TOML, must return non-empty table");
        }
};

QTEST_MAIN(TomlParserTest)
#include "TomlParserTest.moc"
