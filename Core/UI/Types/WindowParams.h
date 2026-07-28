// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QSize>
#include <QString>

// Parameters required to construct MainWindow.
struct WindowParams final {
        QSize   size;
        QString title;
};
