// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

class QString;
class QStringView;

QString singleQuotesToDoubleQuotes(QStringView command);

class ShellRunner final {
public:
        ShellRunner() = delete;
        static void runCommand(const QString& command);
};
