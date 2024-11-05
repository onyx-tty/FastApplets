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
#include <QIcon>
#include <QVBoxLayout>
#include <QString>

class Button
        : public QPushButton {
        Q_OBJECT

protected:
        explicit Button(QWidget* parent,
                        QVBoxLayout* main_layout,
                        const QIcon& button_icon, // TODO default icon
                        const QString& text);
        virtual ~Button() = 0;

public:
        // Workaround that aligns buttons to the left and keeps the text centered (sort of)
        /* It'll likely be removed once I get into actually inheriting from QProxyStyle
           It may be a bit wasteful, however as of now alternative solutions would clutter the code massively */
        void debugAlignIconLeft(QString label_text);
};

#endif // BUTTON_H
