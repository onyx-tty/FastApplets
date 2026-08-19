// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PathContext.h"
#include "Core/Applets/Types/Type.h"

#include <QString>
#include <QStringView>
#include <cstddef>

using config::PathContext;

PathContext::PathContext(QStringView filename, QStringView path_context, char separator) :
        filename(filename.toString()), path_context(path_context.toString()), separator(separator) {
}

PathContext PathContext::operator[](size_t index) const {
        PathContext copy = *this;

        copy.path_context = QString("%1[%2]").arg(path_context).arg(index);

        return copy;
}

PathContext PathContext::operator[](QStringView segment) const {
        PathContext copy = *this;

        // Nothing to separate if empty
        if (!path_context.isEmpty()) { copy.path_context += copy.separator; }

        copy.path_context += segment.toString();

        return copy;
}

QString PathContext::makePath(applet::Type applet) const {
        return QString("in %1, %2%3%4")
                .arg(QString(filename), applet::toString(applet))
                .arg(separator)
                .arg(path_context);
}
