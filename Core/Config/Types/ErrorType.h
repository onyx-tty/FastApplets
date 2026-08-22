// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QDebug>
#include <QStringView>
#include <QtGlobal>
#include <utility>

namespace config {

enum class ErrorType { Missing, InvalidType, InvalidValue, ArrayOutOfBounds };

// Prints error values from given type of error, path corresponding to a node, and filename,
// such as "config.toml" or "keys.toml".
inline void printError(ErrorType error_type, QStringView path, QStringView filename) {
        using enum ErrorType;

        QString pattern = "(%1) %2 %3";

        switch (error_type) {
        case Missing: qWarning().noquote() << pattern.arg(filename, "Missing", path); break;
        case InvalidType:
                qWarning().noquote() << pattern.arg(filename, "Invalid type in", path);
                break;
        case InvalidValue:
                qWarning().noquote() << pattern.arg(filename, "Invalid value in", path);
                break;
        case ArrayOutOfBounds:
                qWarning().noquote() << pattern.arg(filename, "Array out of range", path);
                break;
        default:
                qCritical().noquote()
                        << "Impossible error_type found:" << static_cast<int>(error_type);
                std::unreachable();
        }
}

} // namespace config
