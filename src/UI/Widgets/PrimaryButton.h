/* FastApplets

   Copyright (C) 2026 Łukasz Wrodarczyk

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>. */

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
        // Decouple text and icon using proxy labels
        void alignIconLabel(const QPixmap& icon_pixmap, Qt::Alignment alignment,
                            QSizePolicy size_policy);
        void alignLabel(const QString& text, Qt::Alignment label_alignment);

        QLabel* icon_label;
        QLabel* text_label;

protected:
        explicit PrimaryButton(const QIcon& icon, const QString& text,
                               const PrimaryButtonProperties& properties);
        virtual ~PrimaryButton() = 0;

public:
        QIcon   icon() = delete;
        QString text() const;
};
