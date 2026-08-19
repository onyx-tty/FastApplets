// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Core/Config/Toml/Parse.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QLatin1StringView>
#include <QObject>
#include <QTest>
#include <QTextStream>

using namespace Qt::Literals::StringLiterals;

class ParseTest final : public QObject {
        Q_OBJECT

        static constexpr QLatin1StringView dirpath          = "/tmp/FastAppletsTests"_L1;
        static constexpr QLatin1StringView filename_valid   = "ParseTomlTestOK.toml"_L1;
        static constexpr QLatin1StringView filename_invalid = "ParseTomlTestBAD.toml"_L1;
        static constexpr const char*       contents_valid   = R"(
                [table]
                name = "FastApplets"
                year = 2026
                wip = true
                applets = ["PowerApplet", "ActionApplet"]
                half = 0.5
                )";
        static constexpr const char*       contents_invalid = R"(
                [wrong syntax
                abc = 5
                = 1)";

private slots:
        static void initTestCase() {
                // Verify that /tmp and test directory are accessible
                // before writing anything
                QDir parentdir("/tmp");
                QVERIFY2(parentdir.exists() && parentdir.isReadable()
                                 && QFileInfo(parentdir.path()).isWritable(),
                        "No access to /tmp");

                QVERIFY2(parentdir.mkdir("FastAppletsTests"), "Failed to create test directory");

                // Write minimal valid and invalid TOML files for parser tests
                QFile valid(QDir(dirpath).filePath(filename_valid));
                QVERIFY2(valid.open(QIODevice::WriteOnly | QIODevice::Text),
                        "Failed to create test valid TOML file");

                // Valid
                QTextStream writer(&valid);
                writer << contents_valid;
                valid.close();
                writer.flush();

                // Invalid
                QFile invalid(QDir(dirpath).filePath(filename_invalid));
                QVERIFY2(invalid.open(QIODevice::WriteOnly | QIODevice::Text),
                        "Failed to create test invalid TOML file");
                writer.setDevice(&invalid);
                writer << contents_invalid;
                invalid.close();
        }

        static void cleanupTestCase() {
                // Double-check to guard against path corruption
                QVERIFY2(dirpath.startsWith("/tmp/"_L1), "Test directory must be under /tmp/");

                QDir dir(dirpath);
                if (dir.exists()) {
                        QVERIFY2(dir.removeRecursively(), "Failed to remove test directory");
                }
        }

        static void handlesEmptyFilepath() {
                QVERIFY2(config::parseTomlFile("").empty(),
                        "Empty filepath must return empty toml::table");
        }

        static void handlesMissingFile() {
                QVERIFY2(config::parseTomlFile("/tmp/invalid/path/found.toml").empty(),
                        "Missing file must return empty toml::table");
        }

        static void handlesInvalidToml() {
                const auto invalid = QDir(dirpath).filePath(filename_invalid);

                QVERIFY2(config::parseTomlFile(invalid).empty(),
                        "File with invalid TOML must return empty toml::table");
        }

        static void handlesValidToml() {
                const auto valid = QDir(dirpath).filePath(filename_valid);

                QVERIFY2(!config::parseTomlFile(valid).empty(),
                        "Failed to parse file with valid TOML, must return non-empty table");
        }
};

QTEST_MAIN(ParseTest)
#include "ParseTest.moc"
