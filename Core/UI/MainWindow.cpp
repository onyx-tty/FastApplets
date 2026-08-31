// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "MainWindow.h"
#include "Core/Config/Schema/Config.h"
#include "Core/Config/Schema/Keys.h"
#include "Core/UI/CentralWidget.h"
#include "Core/UI/Widgets/PrimaryButton.h"
#include "Core/UI/Widgets/PrimaryButtons.h"

#include <QMainWindow>
#include <QSize>
#include <QString>
#include <QWidget>
#include <utility>

using config::schema::Config;
using config::schema::Keys;

MainWindow makeMainWindow(const Config& config, const Keys& keys) {
        auto primary_buttons = makePrimaryButtons(
                config.primary_button_params, keys.primary_buttons, nullptr);

        auto* central_widget = new CentralWidget(std::move(primary_buttons), keys.quit,
                config.primary_button_params.behavior, nullptr);

        return MainWindow(config.window_params.title, config.window_params.size, central_widget);
}

MainWindow::MainWindow(const QString& title, QSize size, QWidget* central_widget, QWidget* parent) :
        QMainWindow(parent) {
        setWindowTitle(title);
        resize(size);
        setCentralWidget(central_widget);
}
