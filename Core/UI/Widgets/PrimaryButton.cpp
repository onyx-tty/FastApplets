// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PrimaryButton.h"
#include "Core/Config/ConfigFile/Properties/PrimaryButtonProperties.h"
#include "Core/Config/KeysFile/Types/Keybindings.h"
#include "Core/Shell/ShellRunner.h"
#include "Core/UI/Types/ButtonType.h"

#include <utility>
#include <QFocusEvent>
#include <QLabel>
#include <QObject>
#include <QPaintEvent>
#include <QPushButton>
#include <QStackedLayout>
#include <QStyle>
#include <QStyleOptionButton>
#include <QStylePainter>
#include <QWidget>
#include <Qt>

void PrimaryButton::paintEvent(QPaintEvent*) {
        QStyleOptionButton option;
        initStyleOption(&option);

        // Disable Qt's hover effect
        // TODO: Re-implement
        option.state &= ~QStyle::State_MouseOver;

        if (hasFocus()) {
                // Apply State_HasFocus and State_Sunken for key press
                switch (focus_reason) {
                case Qt::FocusReason::OtherFocusReason:
                        option.state |= QStyle::State_HasFocus;
                        option.state |= QStyle::State_Sunken;
                        break;
                // For anything else, do nothing
                default: break;
                }
        } else {
                // Remove State_HasFocus and State_Sunken for key press
                switch (focus_reason) {
                case Qt::FocusReason::OtherFocusReason:
                        option.state &= ~QStyle::State_HasFocus;
                        option.state &= ~QStyle::State_Sunken;
                        break;
                // For anything else, do nothing
                default: break;
                }
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

PrimaryButton::PrimaryButton(button_type type, const QIcon& icon, const QString& text,
                             keybindings keys, QString command,
                             const PrimaryButtonProperties& properties, QWidget* parent) :
        QPushButton(parent), type(type), keys(std::move(keys)), command(std::move(command)),
        focus_reason(Qt::FocusReason::NoFocusReason) {
        connect(this, &PrimaryButton::clicked, [this]() { ShellRunner::command(this->command); });
        connect(this, &PrimaryButton::clicked, [this]() {
                clearFocus();
                if (auto* parent = this->parentWidget()) {
                        parent->setFocus(Qt::FocusReason::OtherFocusReason);
                }
        });

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

void PrimaryButton::focusInEvent(QFocusEvent* event) {
        QPushButton::focusInEvent(event);
        focus_reason = event->reason();
}

void PrimaryButton::focusOutEvent(QFocusEvent* event) {
        QPushButton::focusOutEvent(event);
        focus_reason = event->reason();
}

QString PrimaryButton::text() const {
        if (!text_label) { return {}; };
        return text_label->text();
}

const keybindings& PrimaryButton::getKeys() const {
        return keys;
}

button_type PrimaryButton::getType() const {
        return type;
}
