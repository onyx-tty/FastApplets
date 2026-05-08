// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PowerButton.h"
#include "Config/ConfigFile/PowerApplet/PowerAppletConfig.h"
#include "CppUtils/Log/QtLog.h"
#include "Shell/ShellRunner.h"
#include "UI/Types/ButtonID.h"
#include "UI/Widgets/PrimaryButton.h"

#include <QBoxLayout>
#include <QIcon>
#include <QObject>
#include <QString>

PowerButton::PowerButton(power_button_id id, const QIcon& icon, const QString& text,
                         const QString& command) :
        PrimaryButton(icon, text, PowerAppletConfig::get().getPrimaryButtonProperties()), id(id) {
        connect(this, &PowerButton::clicked,
                [this, command]() { ShellRunner::runCommand(command); });

        QDEBUG() << QString("Created %1!").arg(text);
}

power_button_id PowerButton::getID() const {
        return id;
}
