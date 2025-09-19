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

#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QString>

class Button : public QPushButton {
        Q_OBJECT

protected:
        bool    is_focused;
        QLabel* debug_text;

        explicit Button(QWidget* parent, QHBoxLayout* main_layout,
                        const QIcon   button_icon, // TODO default icon
                        const QString button_text);
        virtual ~Button() = 0;

        /* Modified reimplementation of paintEvent that allows for manual setting of focus */
        void paintEvent(QPaintEvent*) override;
        /* Modified reimplementation that turns off the distracting mouse hover effect */
        bool event(QEvent* event) override;
        /* Workaround which aligns buttons to a desired position,
           and keeps the text realigned and separate via a proxy label */
        void debugAlignIcon(QString label_text);

public:
        void                  setFocus(bool is_focused);
        const bool            isFocused();
        virtual const QString text() const; /* Returns text from the label, not the button itself */
};

#endif // BUTTON_H
