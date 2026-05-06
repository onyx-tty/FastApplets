// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Config/KeysFile/PowerApplet/PowerAppletKeys.h"
#include "Config/KeysFile/Types/Keybindings.h"
#include "UI/Enums/ButtonIDs.h"
#include "Widgets/PowerButton.h"
#include "Widgets/PowerButtonRecord.h"

#include <vector>
#include <QBoxLayout>
#include <QKeyEvent>
#include <QWidget>
#include <Qt>

class QShowEvent;

class PowerCentralWidget final : public QWidget {
        Q_OBJECT

private:
        PowerButtonRecords createButtons();

        QBoxLayout*        main_layout;
        PowerButtonRecords buttons;

public:
        explicit PowerCentralWidget();
        const QBoxLayout*         getMainLayout() const;
        const PowerButtonRecords& getButtons() const;
        void                      keyPressEvent(QKeyEvent* event) override;
        void                      showEvent(QShowEvent* event);
};
