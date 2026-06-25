// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QMainWindow>
#include <QObject>
#include <QWidget>

class QString;
class QSize;

// Unpacks WindowProperties to set window size and title.
class MainWindow : public QMainWindow {
        Q_OBJECT

public:
        explicit MainWindow(const QString& title, QSize size, QWidget* central_widget,
                            QWidget* parent = nullptr);
};
