// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PrimaryButton.h"
#include "Config/ConfigFile/Properties/PrimaryButtonProperties.h"

#include <QLabel>
#include <QStackedLayout>
#include <QStyle>
#include <QStyleOptionButton>
#include <QStylePainter>
#include <Qt>

void PrimaryButton::paintEvent(QPaintEvent*) {
        QStyleOptionButton option;
        initStyleOption(&option);

        // Disable Qt's hover effect
        option.state &= ~QStyle::State_MouseOver;

        // Visualize the state of focus, apply sunken to increase visibility
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

void PrimaryButton::setTextLabel(const QString& text, Qt::Alignment alignment) {
        if (text_label) { text_label->deleteLater(); }
        text_label = new QLabel(text, this);
        text_label->setAlignment(alignment);
        text_label->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        layout()->addWidget(text_label);
}

void PrimaryButton::setIconLabel(const QPixmap& pixmap, Qt::Alignment alignment,
                                 QSizePolicy size_policy) {
        if (icon_label) { icon_label->deleteLater(); }
        icon_label = new QLabel("", this);
        icon_label->setAlignment(alignment);
        icon_label->setSizePolicy(size_policy);
        icon_label->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        icon_label->setPixmap(pixmap);
        layout()->addWidget(icon_label);
}

PrimaryButton::PrimaryButton(const QIcon& icon, const QString& text,
                             const PrimaryButtonProperties& properties) {
        auto* stacked = new QStackedLayout(this);
        stacked->setStackingMode(QStackedLayout::StackAll);
        setLayout(stacked);

        setIconSize(properties.getIconSize());
        setSizePolicy(properties.getPolicy());
        setAutoDefault(false);
        setTextLabel(text, properties.getTextAlignment());
        setIconLabel(icon.pixmap(properties.getIconSize()), properties.getIconAlignment(),
                     properties.getPolicy());
}

PrimaryButton::~PrimaryButton() = default;

QString PrimaryButton::text() const {
        if (!text_label) { return {}; };
        return text_label->text();
}
