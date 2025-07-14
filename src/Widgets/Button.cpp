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

#include "Button.h"
#include "../Config/SharedLayout.h"

#include <QGridLayout>
#include <QLabel>

/* Initializes a button of choice with uniform design */
// Inheriting constructor defaults from from QPushButton,
// but customizing the icon, icon size and the alignment of that button
Button::Button(QWidget* parent, // TODO Default icon
               QVBoxLayout* main_layout,
               const QIcon& button_icon,
               const QString& text)
        : QPushButton(parent) {
        setIcon(button_icon);
        setIconSize(shared_icon::size);
        setSizePolicy(shared_policy::buttons);
        debugAlignIcon(text);
        main_layout->addWidget(this);
}

Button::~Button() = default;

/* Workaround which aligns buttons to the left, and keeps the text centered via a proxy label */
void Button::debugAlignIcon(QString label_text) {
        setLayout(new QGridLayout);
        setStyleSheet(QString(shared_style::unselected));
        debug_text = new QLabel(label_text, this); // label that acts as a button text replacement
        debug_text->setAlignment(shared_button_alignment::text);
        debug_text->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        layout()->addWidget(debug_text);
}

QString Button::text() const { // Returns text from the label, not the button itself
        return debug_text->text();
}
