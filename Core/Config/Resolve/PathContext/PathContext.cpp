// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PathContext.h"
#include "Core/Applets/Types/Type.h"

#include <cstddef>
#include <string>
#include <string_view>
#include <QString>
#include <QStringView>

using PathContext = config::resolve::PathContext;

PathContext::PathContext(QStringView filename, QStringView path_context, char separator) :
        filename(filename.toString()), path_context(path_context.toString()), separator(separator) {
}

PathContext PathContext::makeCopy() const {
        return *this;
}

PathContext& PathContext::withExtension(std::string_view segment) {
        // Nothing to separate if empty
        if (!path_context.isEmpty()) {
                path_context += separator;
        }

        path_context += QString::fromStdString(std::string(segment));

        return *this;
}

PathContext& PathContext::withExtension(size_t index) {
        path_context = QString("%1[%2]").arg(path_context).arg(index);

        return *this;
}

QString PathContext::makePath(applet::type applet) const {
        return QString("in %1, %2%3%4")
                .arg(QString(filename), applet::toString(applet))
                .arg(separator)
                .arg(path_context);
}
