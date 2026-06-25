// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ActionMainWindow.h"
#include "Core/UI/MainWindow.h"

#include <QSize>
#include <QString>
#include <QWidget>

ActionMainWindow::ActionMainWindow(const QString& title, QSize size, QWidget* central_widget,
                                   QWidget* parent) :
        MainWindow(title, size, central_widget, parent) {}
