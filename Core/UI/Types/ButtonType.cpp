// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ButtonType.h"

#include <QHash>
#include <QStringView>
#include <variant>

template<>
PowerButtonType toPrimaryButtonType<PowerButtonType>(QStringView string) {
        static const QHash<QStringView, PowerButtonType> map = {
                {u"power_off",  PowerButtonType::Shutdown},
                { u"shutdown",  PowerButtonType::Shutdown},
                {   u"reboot",    PowerButtonType::Reboot},
                {  u"suspend",   PowerButtonType::Suspend},
                {u"hibernate", PowerButtonType::Hibernate},
                {  u"log_out",    PowerButtonType::LogOut}
        };

        const auto iter = map.find(string);
        if (iter == map.cend()) { return PowerButtonType::None; }

        return *iter;
}

template<>
PlayerButtonType toPrimaryButtonType<PlayerButtonType>(QStringView string) {
        static const QHash<QStringView, PlayerButtonType> map = {
                {        u"play_pause",        PlayerButtonType::PlayPause},
                {              u"next",             PlayerButtonType::Next},
                {          u"previous",         PlayerButtonType::Previous},
                {         u"volume_up",         PlayerButtonType::VolumeUp},
                {       u"volume_down",       PlayerButtonType::VolumeDown},
                {u"volume_mute_output", PlayerButtonType::VolumeMuteOutput},
                { u"volume_mute_input",  PlayerButtonType::VolumeMuteInput}
        };

        const auto iter = map.find(string);
        if (iter == map.cend()) { return PlayerButtonType::None; }

        return *iter;
}

// Returns monostate for any other type.
// T and QStringView are unused, they are only there to match
// the number of params defined in the template.
template<>
std::monostate toPrimaryButtonType<std::monostate>(QStringView string) {
        return std::monostate{};
}
