// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <string_view>
#include <QString>
#include <QStringView>

class PathContext final {
private:
        QString path_context;
        QString filename;
        char    separator;

public:
        explicit PathContext(QStringView filename, QStringView path_context, char separator = '.');
        QString     makePath(std::string_view scope) const;
        PathContext getExtended(std::string_view segment) const;
        PathContext getExtended(size_t index) const;
};
