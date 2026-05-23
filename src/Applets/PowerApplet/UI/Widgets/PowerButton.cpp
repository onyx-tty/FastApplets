// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PowerButton.h"
#include "Config/ConfigFile/PowerApplet/PowerAppletConfig.h"
#include "Config/KeysFile/Types/Keybindings.h"
#include "CppUtils/Log/QtLog.h"
#include "Shell/ShellRunner.h"
#include "UI/Types/ButtonType.h"
#include "UI/Widgets/PrimaryButton.h"

#include <QBoxLayout>
#include <QIcon>
#include <QObject>
#include <QString>
#include <QWidget>

PowerButton::PowerButton(power_button_type type, const QIcon& icon, const QString& text,
                         const keybindings& keys, const QString& command, QWidget* parent) :
        PrimaryButton(icon, text, PowerAppletConfig::get().getPrimaryButtonProperties(), parent),
        keys(keys), type(type) {
        connect(this, &PowerButton::clicked, [this, command]() { ShellRunner::command(command); });

        QDEBUG() << QString("Created %1!").arg(text);
}

power_button_type PowerButton::getType() const {
        return type;
}

const keybindings& PowerButton::getKeys() const {
        return keys;
}
