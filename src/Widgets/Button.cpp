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
Button::Button(QWidget* parent, // TODO default icon
               QVBoxLayout* main_layout,
               const QIcon& button_icon,
               const QString& text)
        : QPushButton(parent) {
        setIcon(button_icon);
        setIconSize(shared_icon::size);
        setSizePolicy(shared_policy::buttons);
        debugAlignIconLeft(text);
        main_layout->addWidget(this);
}

Button::~Button() = default;

// Workaround that aligns buttons to the left and keeps the text centered (sort of)
/* It'll likely be removed once I get into actually inheriting from QProxyStyle
   It may be a bit wasteful, however as of now alternative solutions would clutter the code massively */
void Button::debugAlignIconLeft(QString label_text) {
        QString style_sheet = "text-align: left;";
        setLayout(new QGridLayout);
        setStyleSheet(QString(style_sheet));
        QLabel* debug_text = new QLabel(label_text, this);
        debug_text->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        debug_text->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        layout()->addWidget(debug_text);
}
