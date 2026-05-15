// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QPushButton>
#include <Qt>

class PrimaryButtonProperties;
class QIcon;
class QLabel;
class QPaintEvent;
class QPixmap;
class QSizePolicy;
class QString;

// Base class for applet buttons with custom icon/text positioning and keyboard staging.
//
// Staging: a safety mechanism requiring two key presses to activate destructive actions
//          (e.g. shutdown). First press stages the button (visual focus), second
//          activates it. Quit key cancels staging.
//          TODO: The keyPressEvent override is in PowerCentralWidget, which should have
//                a base class to store staging and their docs in one place.
//          TODO: All other keys should cancel staging, not just quit key
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

        QLabel* text_label = nullptr;
        QLabel* icon_label = nullptr;

protected:
        // Parameters:
        //   icon       - Button icon (scaled to properties.getIconSize())
        //   text       - Button label text
        //   properties - Visual properties (alignments, icon size, size policy)
        explicit PrimaryButton(const QIcon& icon, const QString& text,
                               const PrimaryButtonProperties& properties);
        virtual ~PrimaryButton() = 0;

public:
        // QPushButton::icon() would return garbage because the inherited icon
        // storage is not used, and a separate QLabel is used instead. Deleted
        // to prevent misuse.
        QIcon icon() = delete;

        // Returns text from the custom QLabel.
        QString text() const;
};
