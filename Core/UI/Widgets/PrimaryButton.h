// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <vector>
#include <QObject>
#include <QPushButton>
#include <Qt>

#include "Core/Config/KeysFile/Types/Keybindings.h"
#include "Core/UI/Types/ButtonType.h"

namespace config::schema::properties {
class PrimaryButton;
} // namespace config::schema::properties

class PrimaryButton;
class PrimaryButtonParams;
class QIcon;
class QLabel;
class QPaintEvent;
class QPixmap;
class QSizePolicy;
class QString;
class QFocusEvent;
class QWidget;

// Constructs PrimaryButtons from given PrimaryButtonParams, assigns visual properties from
// config::schema::properties::PrimaryButton, and keybindings from keys, with fallback default_keys
//
// Keybinding resolution order:
// 1. User-configured keys (keys).
// 2. Default keys (default_keys).
// 3. Qt::Key_unknown if the defaults are exhausted.
//
// WARNING: If a resolved key is already bound elsewhere, both buttons
//          will share it silently. A global keybinding validation pass is not
//          implemented yet.
//
// Returns a vector containing every created button.
// Calls qFatal instead if no button params are found.
// TODO: This function does too much. It should not resolve keys on top of button construction.
std::vector<PrimaryButton*> makePrimaryButtons(
        const std::vector<PrimaryButtonParams>&          params,
        const config::schema::properties::PrimaryButton& properties,
        const std::vector<keybindings>& keys, const std::vector<keybindings>& default_keys,
        QWidget* parent);

// Main button widget used for core functionality.
// It sets given text and icon, and aligns them according to alignments passed in
// config::schema::properties::PrimaryButton. It also sets passed 'command' argument to execute on button click.
class PrimaryButton : public QPushButton {
        Q_OBJECT

private:
        // Custom paint to visualize staging (keyboard focus) and disable hover.
        // Staged buttons show as sunken/focused. Mouse hover is disabled to avoid
        // interfering with keyboard navigation.
        void paintEvent(QPaintEvent*) override;

        // QPushButton forces icon and text to share alignment. To position them
        // independently, this class uses QLabels instead of complete text().
        void setTextLabel(const QString& text, Qt::Alignment alignment);
        void setIconLabel(const QPixmap& pixmap, Qt::Alignment alignment, QSizePolicy size_policy);

        // Caches latest encountered focus reason for use by paintEvent.
        // It exists so that paintEvent can discern between key and mouse and
        // apply the style appropriate for each.
        Qt::FocusReason   focus_reason;
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
        // TODO: Pass a single PrimaryButtonParams argument
        explicit PrimaryButton(button_type type, const QIcon& icon, const QString& text,
                               keybindings keys, QString command,
                               const config::schema::properties::PrimaryButton& properties,
                               QWidget*                                         parent);

        // Wrapper propagating event->reason() to focus_reason for use by paintEvent.
        void focusInEvent(QFocusEvent* event) override;

        // Wrapper propagating event->reason() to focus_reason for use by paintEvent.
        void focusOutEvent(QFocusEvent* event) override;

        // QPushButton::icon() would return garbage because the inherited icon
        // storage is not used, and a separate QLabel is used instead. Deleted
        // to prevent misuse.
        QIcon icon() = delete;

        // Returns text from the custom QLabel.
        QString text() const;

        const keybindings& getKeys() const;
        button_type        getType() const;
};
