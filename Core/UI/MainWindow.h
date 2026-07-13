// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QMainWindow>
#include <QObject>
#include <QWidget>

class MainWindow;
class Config;
class Keys;
class QString;
class QSize;

// Initializes applet UI from scratch, including PrimaryButtons, CentralWidget, and MainWindow.
// MainWindow is constructed from provided config, keys, and default_keys.
MainWindow makeMainWindow(const Config& config, const Keys& keys, const Keys& default_keys);

// Wrapper around QMainWindow that sets title, size, and central_widget during construction.
class MainWindow : public QMainWindow {
        Q_OBJECT

public:
        explicit MainWindow(const QString& title, QSize size, QWidget* central_widget,
                            QWidget* parent = nullptr);
};
