// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/KeysFile/Types/Keybindings.h"
#include "Core/UI/Widgets/PrimaryButton.h"

#include <QObject>

class PrimaryButtonProperties;
class QBoxLayout;
class QIcon;
class QString;
class QWidget;

// Action button with associated icon and keybindings.
//   type:       Determines which icon to use and which keybindings can activate it.
//   icon:       Button icon (scaled to properties.getIconSize()).
//   text:       Button label text.
//   keys:       Key combinations that trigger this button.
//   command:    The actual system action to execute
//   properties: Visual properties (alignments, icon size, size policy).
class ActionButton final : public PrimaryButton {
        Q_OBJECT

public:
        explicit ActionButton(const QIcon& icon, const QString& text, keybindings keys,
                              const QString& command, const PrimaryButtonProperties& properties,
                              QWidget* parent);
};
