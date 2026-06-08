// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ButtonType.h"

#include <QHash>
#include <QStringView>

template<>
power_button_type toPrimaryButtonType<power_button_type>(QStringView string) {
        static const QHash<QStringView, power_button_type> map =
                {{u"poweroff", power_button_type::shutdown},
                 {u"shutdown", power_button_type::shutdown},
                 {u"reboot", power_button_type::reboot},
                 {u"suspend", power_button_type::suspend},
                 {u"hibernate", power_button_type::hibernate}};

        const auto iter = map.find(string);
        if (iter == map.cend()) { return power_button_type::none; }

        return *iter;
}
