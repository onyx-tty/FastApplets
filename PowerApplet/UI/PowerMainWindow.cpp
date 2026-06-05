// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PowerMainWindow.h"
#include "Core/Config/ConfigFile/Properties/WindowProperties.h"
#include "Core/UI/MainWindow.h"
#include "PowerCentralWidget.h"

#include <QWidget>

// TODO: Accept properties as param for dependency injection
PowerMainWindow::PowerMainWindow(const WindowProperties& properties, QWidget* parent) :
        MainWindow(properties, parent) {
        resize(properties.getSize());
        setCentralWidget(new PowerCentralWidget(this));
}
