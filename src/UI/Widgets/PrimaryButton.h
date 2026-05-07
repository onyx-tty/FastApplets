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
        // Modified re-implementation of paintEvent that allows for manual setting of focus
        void paintEvent(QPaintEvent*) override;
        void setTextLabel(const QString& text, Qt::Alignment label_alignment);
        void setIconLabel(const QPixmap& icon_pixmap, Qt::Alignment alignment,
                          QSizePolicy size_policy);

        QLabel* icon_label = nullptr;
        QLabel* text_label = nullptr;

protected:
        explicit PrimaryButton(const QIcon& icon, const QString& text,
                               const PrimaryButtonProperties& properties);
        virtual ~PrimaryButton() = 0;

public:
        QIcon   icon() = delete;
        QString text() const;
};
