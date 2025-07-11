/* Pretty Applets

   Copyright (C) 2024 Łukasz Wrodarczyk

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

#ifndef BUTTON_H
#define BUTTON_H

#include <QPushButton>
#include <QLabel>
#include <QIcon>
#include <QVBoxLayout>
#include <QString>

class Button
        : public QPushButton {
        Q_OBJECT

protected:
        QLabel* debug_text;

        explicit Button(QWidget* parent,
                        QVBoxLayout* main_layout,
                        const QIcon& button_icon, // TODO default icon
                        const QString& text);
        virtual ~Button() = 0;

        /* Workaround which aligns buttons to the left, and keeps the text centered via a proxy label */
        void debugAlignIconLeft(QString label_text);

public:
        virtual QString text() const;
};

#endif // BUTTON_H
