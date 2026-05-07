// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QBoxLayout>
#include <QEvent>
#include <QIcon>
#include <QLabel>
#include <QPaintEvent>
#include <QPushButton>
#include <QString>

class PrimaryButtonProperties;

class PrimaryButton : public QPushButton {
        Q_OBJECT

private:
        // Represent focus as State_Sunken to visualize staging
        void paintEvent(QPaintEvent*) override;

        // Support separate alignments for text and icon by storing both as separate labels
        // and aligning them separately
        void setTextLabel(const QString& text, Qt::Alignment alignment);
        void setIconLabel(const QPixmap& pixmap, Qt::Alignment alignment, QSizePolicy size_policy);

        QLabel* text_label = nullptr;
        QLabel* icon_label = nullptr;

protected:
        explicit PrimaryButton(const QIcon& icon, const QString& text,
                               const PrimaryButtonProperties& properties);
        virtual ~PrimaryButton() = 0;

public:
        // Delete icon() accessor to avoid fetching the unused icon
        QIcon   icon() = delete;
        QString text() const;
};
