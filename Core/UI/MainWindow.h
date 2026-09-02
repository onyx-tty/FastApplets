// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Shell/Types/ShellContext.h"

#include <QMainWindow>
#include <QObject>
#include <QWidget>

namespace config::schema {
class Config;
class Keys;
} // namespace config::schema

class MainWindow;
class QString;
class QSize;

// Initializes applet UI from scratch, including PrimaryButtons, CentralWidget, and MainWindow.
// MainWindow is constructed from provided config and keys.
[[nodiscard]] MainWindow makeMainWindow(
        const config::schema::Config& config, const config::schema::Keys& keys);

// Wrapper around QMainWindow that sets title, size, and central_widget during construction.
class MainWindow : public QMainWindow {
        Q_OBJECT

public:
        explicit MainWindow(const QString& title, QSize size, QWidget* central_widget,
                QWidget* parent = nullptr);
};
