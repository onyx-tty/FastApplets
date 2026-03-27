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

#include "Button.h"
#include "Config/Config/Properties/PrimaryButtonProperties.h"
#include "Core/Log.h"

#include <QGridLayout>
#include <QStyleOptionButton>
#include <QStylePainter>

namespace {
QString stylesheet = "text-align: center top;";
} // namespace

void Button::paintEvent(QPaintEvent*) {
        QStyleOptionButton option;
        initStyleOption(&option);

        // Disable Qt's hover effect
        option.state &= ~QStyle::State_MouseOver;

        // Smart apply the focus state
        if (hasFocus()) {
                option.state |= QStyle::State_HasFocus;
                option.state |= QStyle::State_Sunken;
        } else {
                option.state &= ~QStyle::State_HasFocus;
                option.state &= ~QStyle::State_Sunken;
        }

        QStylePainter painter(this);
        style()->drawControl(QStyle::CE_PushButton, &option, &painter, this);
}

bool Button::event(QEvent* event) {
        switch (event->type()) { // Ignore mouse hover, forward anything else
        case QEvent::HoverMove:
        case QEvent::HoverEnter:
        case QEvent::HoverLeave: return false;
        default:                 return QPushButton::event(event);
        }
}

void Button::alignLabel(const QString& label_text, Qt::Alignment label_alignment) {
        setLayout(new QGridLayout);
        setStyleSheet(stylesheet);
        label = new QLabel(label_text, this); // label that acts as a button text replacement
        label->setAlignment(label_alignment);
        label->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        layout()->addWidget(label);
        QDEBUG() << "Label successfully initialized with text:" << label_text;
}

/* Initializes a button of choice with uniform design */
// Inheriting constructor defaults from from QPushButton,
// but customizing the icon, icon size and the alignment of that button
// TODO Default icon
Button::Button(QBoxLayout* layout, const QIcon& icon, const QString& label,
               const PrimaryButtonProperties& properties) :
        QPushButton(layout ? layout->widget() : nullptr) {
        if (!layout) { QFATAL("Button constructor received a null layout! Bad code!"); }

        setIcon(icon);
        setIconSize(properties.getIconSize());
        setSizePolicy(properties.getPolicy());
        setAutoDefault(false);
        alignLabel(label, properties.getTextAlignment());
        layout->addWidget(this);
}

Button::~Button() = default;

QString Button::text() const {
        return label->text();
}
