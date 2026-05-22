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
void KeysMapper::mapToPowerAppletKeys(const toml::table& power_applet, const toml::table& global,
                                      TKeys& keys) {
        // Confirm that a QApplication instance exists
        if (!QApplication::instance()) { QFATAL("QApplication has not been instantiated yet!"); }

        using namespace Qt::StringLiterals;
        const auto& defaults = TKeys::getDefault();
        QStringView filename = u"keys.toml"_s;

        /* Quit Keys */
        mapQuitKeys(NodePair{.primary = power_applet["quit"], .fallback = global["quit"]},
                    keys.quit, defaults.getQuit(), PathContext{filename, u"quit"_s});

        /* Primary Button Keys */
        mapPrimaryButtonKeys(power_applet["primary_buttons"], keys.primary_button,
                             defaults.getPrimaryButton(),
                             PathContext{filename, u"primary_buttons"_s});
}
