// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "WindowProperties.h"

#include <utility>
#include <QSize>
#include <QString>

WindowProperties::WindowProperties(QSize size, QString title) :
        size(std::move(size)), title(std::move(title)) {}

const QString& WindowProperties::getTitle() const {
        return title;
}

const QSize& WindowProperties::getSize() const {
        return size;
}
