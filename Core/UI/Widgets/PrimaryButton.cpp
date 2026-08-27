// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PrimaryButton.h"
#include "Core/Config/Types/Keybindings.h"
#include "Core/Shell/Shell.h"
#include "Core/UI/Types/ButtonType.h"
#include "Core/UI/Types/PrimaryButtonParams.h"
#include "Core/UI/Types/PrimaryButtonStyle.h"
#include "Core/UI/Types/PrimaryButtons.h"

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
#include <algorithm>
#include <cstddef>
#include <utility>
#include <vector>

PrimaryButton* findPrimaryButton(int key, PrimaryButtons buttons) {
        const auto iter = std::find_if(
                buttons.cbegin(), buttons.cend(), [key](const PrimaryButton* button) -> bool {
                        if (!button) { return false; }
                        return button->getKeys().contains(key);
                });

        return iter != buttons.cend() ? *iter : nullptr;
}

PrimaryButtons makePrimaryButtons(const PrimaryButtonParams& params,
        const std::vector<keybindings>& keys, const std::vector<keybindings>& default_keys,
        QWidget* parent) {
        // TODO If applied key is already used elsewhere, the keybindings will be unpredictable.
        //      For example if for some reason keybinding for primary button 3 is Qt_Key4 and
        //      primary button 4 has missing keybinding, both buttons will be assigned to Qt_Key4.
        //      There should be a validation system in place for all keybindings, for example a
        //      set with all keys which have already been exhausted.
        const auto key_getter = [&params, &keys, &default_keys](size_t i) -> keybindings {
                if (i < keys.size()) { return keys[i]; }
                if (i < default_keys.size()) {
                        qWarning() << QString(
                                "Key for button %1 not found, applying default Qt_Key%1!")
                                              .arg(i + 1);
                        return default_keys[i];
                }

                qCritical() << "Number of buttons exceeds size of default keys! Buttons found:"
                            << params.per_button.size();
                return keybindings(Qt::Key_unknown);
        };

        PrimaryButtons buttons = {};
        buttons.reserve(params.per_button.size());

        for (size_t i = 0; i != params.per_button.size(); ++i) {
                // TODO: Move semantics
                button_type               type    = params.per_button[i].type;
                const QIcon&              icon    = params.per_button[i].icon;
                const QString&            text    = params.per_button[i].text;
                const QString&            command = params.per_button[i].command;
                const PrimaryButtonStyle& style   = params.style;
                keybindings               keys    = key_getter(i);
                auto* button = new PrimaryButton(type, icon, text, keys, command, style, parent);
                buttons.push_back(button);
        }

        if (buttons.empty()) { qFatal("No buttons found!"); }

        return buttons;
}

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
        keybindings keys, QString command, const PrimaryButtonStyle& style, QWidget* parent) :
        QPushButton(parent), type(type), keys(std::move(keys)), command(std::move(command)),
        focus_reason(Qt::FocusReason::NoFocusReason) {
        connect(this, &PrimaryButton::clicked, [this]() { runCommand(this->command); });
        connect(this, &PrimaryButton::clicked, [this]() {
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
