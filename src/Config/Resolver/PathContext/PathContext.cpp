// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PathContext.h"
#include "Applets/Types/AppletRecord.h"
#include "Applets/Types/AppletType.h"

#include <cstddef>
#include <string>
#include <string_view>
#include <QString>
#include <QStringView>

PathContext::PathContext(QStringView filename, QStringView path_context, char separator) :
        filename(filename.toString()), path_context(path_context.toString()), separator(separator) {
}

QString PathContext::makePath(applet::type applet) const {
        return QString("in %1, %2%3%4")
                .arg(QString{filename},
                     QString::fromStdString(std::string{applet::toString(applet)}))
                .arg(separator)
                .arg(path_context);
}

PathContext PathContext::makeExtended(std::string_view segment) const {
        return PathContext{filename,
                           QString("%1%2%3")
                                   .arg(path_context)
                                   .arg(separator)
                                   .arg(QString::fromStdString(std::string{segment})),
                           separator};
}

PathContext PathContext::makeExtended(size_t index) const {
        return PathContext{filename, QString("%1[%2]").arg(path_context).arg(index), separator};
}
