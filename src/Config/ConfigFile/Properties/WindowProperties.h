// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QSize>
#include <QString>

class ConfigMapper;

class WindowProperties final {
private:
        friend class ConfigMapper;

        QSize   size;
        QString title;

public:
        explicit WindowProperties(QSize size = {}, QString title = {});
        const QSize&   getSize() const;
        const QString& getTitle() const;
};
