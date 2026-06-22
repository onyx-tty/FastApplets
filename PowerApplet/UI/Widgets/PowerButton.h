// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/KeysFile/Types/Keybindings.h"
#include "Core/UI/Types/ButtonType.h"
#include "Core/UI/Widgets/PrimaryButton.h"

#include <QObject>

class PrimaryButtonProperties;
class QBoxLayout;
class QIcon;
class QString;
class QWidget;

// Power button with associated icon and keybindings.
//   type:       Determines which icon to use and which keybindings can activate it.
//   icon:       Button icon (scaled to properties.getIconSize()).
//   text:       Button label text.
//   command:    The actual system action to execute (shutdown, reboot, etc.)
//   properties: Visual properties (alignments, icon size, size policy).
class PowerButton final : public PrimaryButton {
        Q_OBJECT

private:
        const power_button_type type;

public:
        explicit PowerButton(power_button_type type, const QIcon& icon, const QString& text,
                             keybindings keys, const QString& command,
                             const PrimaryButtonProperties& properties, QWidget* parent);
        power_button_type getType() const;
};
