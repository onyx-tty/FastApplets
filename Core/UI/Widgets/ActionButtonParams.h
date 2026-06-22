// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QIcon>
#include <QString>

// Parameters required to construct an ActionButton.
// Currently id, text, command, come from config, icon is assigned from id.
struct ActionButtonParams final {
        QString            text;
        QString            command;
        QIcon              icon;
};
