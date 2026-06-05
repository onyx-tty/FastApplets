// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ButtonType.h"

#include <QHash>
#include <QString>

template<>
power_button_type toPrimaryButtonType<power_button_type>(const QString& string) {
        static const QHash<QString, power_button_type> map =
                {{"poweroff", power_button_type::shutdown},
                 {"shutdown", power_button_type::shutdown},
                 {"reboot", power_button_type::reboot},
                 {"suspend", power_button_type::suspend},
                 {"hibernate", power_button_type::hibernate}};

        const auto iter = map.find(string);
        if (iter == map.cend()) { return power_button_type::none; }

        return *iter;
}
