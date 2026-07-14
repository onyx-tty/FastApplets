// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QSize>
#include <QString>

class ConfigMapper;

namespace config::schema::properties {

// Stores properties used by MainWindow.
class Window final {
private:
        friend class ::ConfigMapper;

        QSize   size;
        QString title;

public:
        explicit Window(QSize size = {}, QString title = {});
        const QSize&   getSize() const;
        const QString& getTitle() const;
};

} // namespace config::schema::properties
