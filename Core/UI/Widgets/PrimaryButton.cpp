// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PrimaryButton.h"
#include "Core/Config/Types/Keybindings.h"
#include "Core/Shell/Shell.h"
#include "Core/Shell/Types/ShellContext.h"
#include "Core/UI/Types/ButtonType.h"
#include "Core/UI/Types/PrimaryButtonStyle.h"

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
#include <utility>

void PrimaryButton::paintEvent(QPaintEvent*) {
        QStyleOptionButton option;
        initStyleOption(&option);

        // Disable Qt's hover effect
        // TODO: Re-implement the MouseOver state
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

void PrimaryButton::setLabel(QLabel* label, const QString& text, const QPixmap& pixmap,
        Qt::Alignment alignment, QSizePolicy size_policy) {
        // If layout is somehow invalid then something in the initialization logic went
        // very wrong. Instability in the application is guaranteed at that point.
        if (!layout()) { qFatal("Called with no layout"); }

        if (label) {
                layout()->removeWidget(label);
                label->deleteLater();
                label = nullptr;
        }

        label = new QLabel(this);

        label->setText(text);
        if (!pixmap.isNull()) { label->setPixmap(pixmap); }
        label->setAlignment(alignment);
        label->setSizePolicy(size_policy);
        label->setAttribute(Qt::WA_TransparentForMouseEvents, true);

        layout()->addWidget(label);
}

PrimaryButton::PrimaryButton(button_type type, const QIcon& icon, const QString& text,
        keybindings keys, QString command, const PrimaryButtonStyle& style, ShellContext context,
        QWidget* parent) :
        QPushButton(parent), type(type), keys(std::move(keys)), command(std::move(command)),
        focus_reason(Qt::FocusReason::NoFocusReason) {
        connect(this, &PrimaryButton::clicked,
                [this, context]() { runCommand(this->command, context); });
        connect(this, &PrimaryButton::clicked, [this]() {
                // Resets focus on button click to prevent it from remaining
                // and conflicting with the stage-then-confirm key controls.
                clearFocus();
                if (auto* parent = this->parentWidget()) {
                        parent->setFocus(Qt::FocusReason::OtherFocusReason);
                }
        });

        auto* stacked = new QStackedLayout(this);
        stacked->setStackingMode(QStackedLayout::StackAll);
        setLayout(stacked);

        setIconSize(style.icon_size);
        setSizePolicy(style.policy);
        setAutoDefault(false);
        setLabel(text_label, text, {}, style.text_alignment, style.policy);
        setLabel(icon_label, {}, icon.pixmap(style.icon_size), style.icon_alignment, style.policy);
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

const keybindings& PrimaryButton::getKeys() const { return keys; }

button_type PrimaryButton::getType() const { return type; }
