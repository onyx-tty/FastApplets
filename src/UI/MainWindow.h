// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QMainWindow>
#include <QObject>

class WindowProperties;

// Unpacks WindowProperties to set window size and title.
class MainWindow : public QMainWindow {
        Q_OBJECT

protected:
        explicit MainWindow(const WindowProperties& properties);
        virtual ~MainWindow() = 0;
};
