// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QSize>
#include <QString>

namespace config {
class ConfigMapper;
} // namespace config

namespace config::schema::properties {

// Stores properties used by MainWindow.
class Window final {
private:
        friend class config::ConfigMapper;

        QSize   size;
        QString title;

public:
        explicit Window(QSize size = {}, QString title = {});
        [[nodiscard]] const QSize&   getSize() const;
        [[nodiscard]] const QString& getTitle() const;
};

} // namespace config::schema::properties
