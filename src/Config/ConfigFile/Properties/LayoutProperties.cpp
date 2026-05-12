// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "LayoutProperties.h"
#include "CppUtils/Log/QtLog.h"
#include "UI/Types/ButtonType.h"
#include "UI/Widgets/PowerButtonParams.h"

#include <utility>
#include <vector>
#include <QIcon>
#include <QResource>
#include <QString>

QIcon iconFor(power_button_type id) {
        Q_INIT_RESOURCE(Icons);

        switch (id) {
        case power_button_type::shutdown:  return QIcon{":/Icons/Power/shutdown.svg"};
        case power_button_type::reboot:    return QIcon{":/Icons/Power/reboot.svg"};
        case power_button_type::suspend:   return QIcon{":/Icons/Power/suspend.svg"};
        case power_button_type::hibernate: return QIcon{":/Icons/Power/hibernate.svg"};
        }

        return {};
}

LayoutProperties::LayoutProperties(std::vector<PowerButtonParams> power_buttons) :
        power_buttons(std::move(power_buttons)) {}

const std::vector<PowerButtonParams>& LayoutProperties::getPowerButtons() const {
        return power_buttons;
}
