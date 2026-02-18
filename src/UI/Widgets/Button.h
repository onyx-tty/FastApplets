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

// TODO Restore mouse click effect
class Button : public QPushButton {
        Q_OBJECT

protected:
        // TODO Potentially not needed
        bool    is_focused;
        // TODO Rename debug_ext
        QLabel* debug_text;

        explicit Button(QBoxLayout*    layout,
                        const QIcon&   icon, // TODO default icon
                        const QString& text);
        virtual ~Button() = 0;

        // Modified re-implementation of paintEvent that allows for manual setting of focus
        void paintEvent(QPaintEvent*) override;
        // Modified re-implementation that turns off the distracting mouse hover effect
        // TODO Mouse hover on/off depending on the config setting
        bool event(QEvent* event) override;
        // Workaround which aligns buttons to a desired position,
        // and keeps the text realigned and separate via a proxy label
        void debugAlignIcon(const QString& label_text);

public:
        void    setFocus(bool is_focused);
        bool    isFocused() const;
        QString text() const; // Returns text from the label, not the button itself
};
