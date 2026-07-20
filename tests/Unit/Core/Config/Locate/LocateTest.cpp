// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Core/Config/Locate/Locate.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QLatin1StringView>
#include <QObject>
#include <QTest>
#include <QTextStream>

using namespace Qt::Literals::StringLiterals;

class LocateTest final : public QObject {
        Q_OBJECT

        static constexpr QLatin1StringView dirpath  = "/tmp/FastAppletsTests"_L1;
        static constexpr QLatin1StringView filename = "LocateTomlTest.toml"_L1;

private slots:
        static void initTestCase() {
                // Verify that /tmp and test directory are accessible
                // before writing anything
                QDir parentdir("/tmp");
                QVERIFY2(parentdir.exists() && parentdir.isReadable()
                                 && QFileInfo(parentdir.path()).isWritable(),
                         "No access to /tmp");

                QVERIFY2(parentdir.mkdir("FastAppletsTests"), "Failed to create test directory");

                // Create a minimal file
                QFile file(QDir(dirpath).filePath(filename));
                QVERIFY2(file.open(QIODevice::WriteOnly | QIODevice::Text), "Failed to create file");
                file.close(); // empty file is good enough
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
                const auto filepaths = config::locateFiles(""_L1);
                for (const auto& filepath : {filepaths.config, filepaths.keys}) {
                        QVERIFY2(filepath.isEmpty(), QString("Filepath %1 must be empty")
                                                             .arg(filepath)
                                                             .toStdString()
                                                             .c_str());
                }
        }

        static void handlesMissingFile() {
                const auto filepaths = config::locateFiles("/tmp/invalid/path/found.toml"_L1);
                for (const auto& filepath : {filepaths.config, filepaths.keys}) {
                        QVERIFY2(filepath.isEmpty(),
                                 QString("Missing file %1 must return empty strings")
                                         .arg(filepath)
                                         .toStdString()
                                         .c_str());
                }
        }
};

QTEST_MAIN(LocateTest)
#include "LocateTest.moc"
