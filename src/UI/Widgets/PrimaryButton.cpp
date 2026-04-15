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

#include "PrimaryButton.h"
#include "Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Log/Log.h"

#include <QStackedLayout>
#include <QStyleOptionButton>
#include <QStylePainter>

void PrimaryButton::paintEvent(QPaintEvent*) {
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

void PrimaryButton::alignLabel(const QString& label_text, Qt::Alignment label_alignment) {
        auto* layout = new QStackedLayout(this);
        layout->setStackingMode(QStackedLayout::StackAll);
        setLayout(layout);

        label = new QLabel(label_text, this); // label that acts as a button text replacement
        label->setAlignment(label_alignment);
        label->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        layout->addWidget(label);

        QDEBUG() << "Label successfully initialized with text:" << label_text;
}

void PrimaryButton::alignIconLabel(const QPixmap& icon_pixmap, Qt::Alignment alignment,
                                   QSizePolicy size_policy) {
        icon_label = new QLabel("", this);
        icon_label->setAlignment(alignment);
        icon_label->setSizePolicy(size_policy);
        icon_label->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        icon_label->setPixmap(icon_pixmap);
        layout()->addWidget(icon_label);
}

/* Initializes a button of choice with uniform design */
// Inheriting constructor defaults from from QPushButton,
// but customizing the icon, icon size and the alignment of that button
// TODO Default icon
PrimaryButton::PrimaryButton(QBoxLayout* layout, const QIcon& icon, const QString& label,
                             const PrimaryButtonProperties& properties) :
        QPushButton(layout ? layout->widget() : nullptr) {
        if (!layout) { QFATAL("Button constructor received a null layout! Bad code!"); }

        setIconSize(properties.getIconSize());
        setSizePolicy(properties.getPolicy());
        setAutoDefault(false);
        alignLabel(label, properties.getTextAlignment());
        alignIconLabel(icon.pixmap(properties.getIconSize()), properties.getIconAlignment(),
                       properties.getPolicy());

        layout->addWidget(this);
}

PrimaryButton::~PrimaryButton() = default;

QString PrimaryButton::text() const {
        return label->text();
}
