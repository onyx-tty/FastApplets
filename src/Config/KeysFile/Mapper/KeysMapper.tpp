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

#include "Config/Resolvers/Resolvers.h"
#include "Config/TOML/Types/NodePair.h"
#include "CppUtils/Log/QtLog.h"
#include "KeysMapper.h"

#include <toml++/toml.hpp>
#include <QApplication>
#include <QStringLiteral>
#include <Qt>

template<typename TKeys>
void KeysMapper::mapToPowerAppletKeys(const toml::table& power_applet_table,
                                      const toml::table& global_table, TKeys& keys) {
        // Confirm that a QApplication instance exists
        if (!QApplication::instance()) { QFATAL("QApplication has not been instantiated yet!"); }

        using namespace Qt::StringLiterals;
        const auto& defaults = TKeys::getDefault();
        QStringView filename = u"keys.toml"_s;

        /* Quit Keys */
        mapQuitKeys(NodePair{power_applet_table["quit"], global_table["quit"]}, keys.quit_keys,
                    defaults.getQuitKeys(), PathContext{filename, u"quit"_s});

        /* Primary Button Keys */
        mapPrimaryButtonKeys(power_applet_table["primary_buttons"], keys.primary_button_keys,
                             defaults.getPrimaryButtonKeys(),
                             PathContext{filename, u"primary_buttons"_s});
}
