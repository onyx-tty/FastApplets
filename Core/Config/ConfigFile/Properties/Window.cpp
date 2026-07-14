// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Window.h"

#include <utility>
#include <QSize>
#include <QString>

using Window = config::schema::properties::Window;

Window::Window(QSize size, QString title) : size(std::move(size)), title(std::move(title)) {}

const QString& Window::getTitle() const {
        return title;
}

const QSize& Window::getSize() const {
        return size;
}
