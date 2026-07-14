// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "WindowProperties.h"

#include <utility>
#include <QSize>
#include <QString>

config::schema::properties::Window::Window(QSize size, QString title) :
        size(std::move(size)), title(std::move(title)) {}

const QString& config::schema::properties::Window::getTitle() const {
        return title;
}

const QSize& config::schema::properties::Window::getSize() const {
        return size;
}
