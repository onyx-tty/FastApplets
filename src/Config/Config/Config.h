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

#include "UI/Widgets/PrimaryButtonData.h"

#include <vector>
#include <QSize>
#include <QSizePolicy>
#include <QString>

class ConfigMapper;

class Config final {
public:
        class WindowProperties final {
        private:
                friend class ConfigMapper;

                QSize   size;
                QString title;

        public:
                WindowProperties(QSize size = {}, QString title = {});
                const QSize&   getSize() const;
                const QString& getTitle() const;
        };

        class WindowLayoutProperties final {
        private:
                friend class ConfigMapper;

                std::vector<PrimaryButtonData> primary_power_buttons;

        public:
                WindowLayoutProperties(std::vector<PrimaryButtonData> primary_power_buttons = {});
                const std::vector<PrimaryButtonData>& getPrimaryPowerButtons() const;
        };

        class PrimaryButtonProperties final {
        private:
                friend class ConfigMapper;

                Qt::Alignment text_alignment;
                Qt::Alignment icon_alignment;
                QSize         icon_size;
                QSizePolicy   policy;

        public:
                PrimaryButtonProperties(Qt::Alignment text_alignment = {},
                                        Qt::Alignment icon_alignment = {}, QSize icon_size = {},
                                        QSizePolicy policy = {});
                const Qt::Alignment& getTextAlignment() const;
                const Qt::Alignment& getIconAlignment() const;
                const QSize&         getIconSize() const;
                const QSizePolicy&   getPolicy() const;
        };

        class EnvironmentProperties final {
        private:
                friend class ConfigMapper;

                bool dbus_mode;

        public:
                EnvironmentProperties(bool dbus_mode = false);
                const bool& getDBusMode() const;
        };

        Config(Config::WindowProperties        window_properties         = {},
               Config::PrimaryButtonProperties primary_button_properties = {},
               Config::WindowLayoutProperties  window_layout_properties  = {},
               Config::EnvironmentProperties   environment_properties    = {});
        // TODO Make this const to avoid overwrites
        static Config&                 getConfig();
        static const Config&           getDefaultConfig();
        const WindowProperties&        getWindowProperties() const;
        const WindowLayoutProperties&  getWindowLayoutProperties() const;
        const PrimaryButtonProperties& getPrimaryButtonProperties() const;
        const EnvironmentProperties&   getEnvironmentProperties() const;

private:
        friend class ConfigMapper;

        WindowProperties        window_properties;
        WindowLayoutProperties  window_layout_properties;
        PrimaryButtonProperties primary_button_properties;
        EnvironmentProperties   environment_properties;
};
