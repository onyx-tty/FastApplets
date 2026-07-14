// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "MainWindow.h"
#include "Core/Config/ConfigFile/Config/Config.h"
#include "Core/Config/KeysFile/Keys/Keys.h"
#include "Core/UI/CentralWidget.h"
#include "Core/UI/Widgets/PrimaryButton.h"

#include <QMainWindow>
#include <QSize>
#include <QString>
#include <QWidget>

namespace {

using Config = config::schema::Config;
using Keys   = config::schema::Keys;

} // namespace

MainWindow makeMainWindow(const Config& config, const Keys& keys, const Keys& default_keys) {
        auto primary_buttons = makePrimaryButtons(config.getLayoutProperties().getPrimaryButtons(),
                                                  config.getPrimaryButtonProperties(),
                                                  keys.getPrimaryButtons(),
                                                  default_keys.getPrimaryButtons(), nullptr);

        auto* central_widget =
                new CentralWidget(std::move(primary_buttons), keys.getQuit(),
                                  config.getPrimaryButtonProperties().getDoubleKeyPress(), nullptr);

        return MainWindow(config.getWindowProperties().getTitle(),
                          config.getWindowProperties().getSize(), central_widget);
}

MainWindow::MainWindow(const QString& title, QSize size, QWidget* central_widget, QWidget* parent) :
        QMainWindow(parent) {
        setWindowTitle(title);
        resize(size);
        setCentralWidget(central_widget);
}
