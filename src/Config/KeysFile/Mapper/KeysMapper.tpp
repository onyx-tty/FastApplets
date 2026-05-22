// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Config/Resolver/PathContext/PathContext.h"
#include "Config/Types/NodePair.h"
#include "CppUtils/Log/QtLog.h"
#include "KeysMapper.h"

#include <toml++/toml.hpp>
#include <QApplication>
#include <QStringView>
#include <Qt>

template<typename TKeys>
TKeys KeysMapper::mapToPowerAppletKeys(const toml::table& power_applet, const toml::table& global) {
        // Confirm that a QApplication instance exists
        if (!QApplication::instance()) { QFATAL("QApplication has not been instantiated yet!"); }

        using namespace Qt::StringLiterals;
        const auto& defaults = TKeys::getDefault();
        QStringView filename = u"keys.toml"_s;

        TKeys keys{};

        /* Quit Keys */
        keys.quit = mapQuitKeys(NodePair{.primary  = power_applet["quit"],
                                         .fallback = global["quit"]},
                                defaults.getQuit(), PathContext{filename, u"quit"_s});

        /* Primary Button Keys */
        keys.primary_button = mapPrimaryButtonKeys(power_applet["primary_buttons"],
                                                   defaults.getPrimaryButton(),
                                                   PathContext{filename, u"primary_buttons"_s});

        return std::move(keys);
}
