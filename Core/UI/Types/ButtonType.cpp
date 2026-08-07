// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ButtonType.h"

#include <variant>
#include <QHash>
#include <QStringView>

template<>
PowerButtonType toPrimaryButtonType<PowerButtonType>(QStringView string) {
        static const QHash<QStringView, PowerButtonType> map =
                {{u"power_off", PowerButtonType::Shutdown},
                 {u"shutdown", PowerButtonType::Shutdown},
                 {u"reboot", PowerButtonType::Reboot},
                 {u"suspend", PowerButtonType::Suspend},
                 {u"hibernate", PowerButtonType::Hibernate},
                 {u"log_out", PowerButtonType::LogOut}};

        const auto iter = map.find(string);
        if (iter == map.cend()) { return PowerButtonType::None; }

        return *iter;
}

template<>
VolumeButtonType toPrimaryButtonType<VolumeButtonType>(QStringView string) {
        static const QHash<QStringView, VolumeButtonType> map =
                {{u"play_pause", VolumeButtonType::PlayPause},
                 {u"next", VolumeButtonType::Next},
                 {u"previous", VolumeButtonType::Previous},
                 {u"volume_up", VolumeButtonType::VolumeUp},
                 {u"volume_down", VolumeButtonType::VolumeDown},
                 {u"volume_mute_output", VolumeButtonType::VolumeMuteOutput},
                 {u"volume_mute_input", VolumeButtonType::VolumeMuteInput}};

        const auto iter = map.find(string);
        if (iter == map.cend()) { return VolumeButtonType::None; }

        return *iter;
}

// Returns monostate for any other type.
// T and QStringView are unused, they are only there to match
// the number of params defined in the template.
template<>
std::monostate toPrimaryButtonType<std::monostate>(QStringView string) {
        return std::monostate{};
}
