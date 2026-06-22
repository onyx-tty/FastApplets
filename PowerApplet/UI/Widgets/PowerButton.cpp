// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PowerButton.h"

#include "Core/Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"
#include "Core/Shell/ShellRunner.h"
#include "Core/UI/Types/ButtonType.h"
#include "Core/UI/Widgets/PrimaryButton.h"

#include <utility>
#include <QBoxLayout>
#include <QDebug>
#include <QIcon>
#include <QObject>
#include <QString>
#include <QWidget>
#include <QtGlobal>

PowerButton::PowerButton(power_button_type type, const QIcon& icon, const QString& text,
                         keybindings keys, const QString& command,
                         const PrimaryButtonProperties& properties, QWidget* parent) :
        PrimaryButton(icon, text, std::move(keys), properties, parent), type(type) {
        // TODO: command gets copied thrice, fix that
        connect(this, &PowerButton::clicked, [this, command]() { ShellRunner::command(command); });

        qDebug() << QString("Created %1!").arg(text);
}

power_button_type PowerButton::getType() const {
        return type;
}
