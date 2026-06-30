// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QPushButton>
#include <Qt>

#include "Core/Config/KeysFile/Types/Keybindings.h"
#include "Core/UI/Types/ButtonType.h"

class PrimaryButtonProperties;
class QIcon;
class QLabel;
class QPaintEvent;
class QPixmap;
class QSizePolicy;
class QString;
class QWidget;

// Main button widget used for core functionality.
// It sets given text and icon, and aligns them according to alignments passed in
// PrimaryButtonProperties. It also sets passed 'command' argument to execute on button click.
class PrimaryButton : public QPushButton {
        Q_OBJECT

private:
        // Custom paint to visualize staging (keyboard focus) and disable hover.
        // Staged buttons show as sunken/focused. Mouse hover is disabled to avoid
        // interfering with keyboard navigation.
        //
        // Bug: Mouse click sets State_Sunken that persists until quit key is pressed.
        void paintEvent(QPaintEvent*) override;

        // QPushButton forces icon and text to share alignment. To position them
        // independently, this class uses QLabels instead of complete text().
        void setTextLabel(const QString& text, Qt::Alignment alignment);
        void setIconLabel(const QPixmap& pixmap, Qt::Alignment alignment, QSizePolicy size_policy);

        const button_type type;
        const keybindings keys;
        const QString     command;

        QLabel* text_label = nullptr;
        QLabel* icon_label = nullptr;

public:
        // Parameters:
        //   type:       Determines icon, as well as fallback text, keys, and command.
        //   icon:       Button icon (scaled to properties.getIconSize()).
        //   text:       Button label text.
        //   keys:       Key combinations that trigger this button.
        //   properties: Visual properties (alignments, icon size, size policy).
        explicit PrimaryButton(button_type type, const QIcon& icon, const QString& text,
                               keybindings keys, QString command,
                               const PrimaryButtonProperties& properties, QWidget* parent);

        // QPushButton::icon() would return garbage because the inherited icon
        // storage is not used, and a separate QLabel is used instead. Deleted
        // to prevent misuse.
        QIcon icon() = delete;

        // Returns text from the custom QLabel.
        QString text() const;

        const keybindings& getKeys() const;
        button_type        getType() const;
};
