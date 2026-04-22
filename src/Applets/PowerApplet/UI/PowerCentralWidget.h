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
};
