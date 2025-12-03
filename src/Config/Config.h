/* FastApplets

   Copyright (C) 2025 Łukasz Wrodarczyk

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

#include "PrimaryButtonData.h"

#include <QSize>
#include <QSizePolicy>

#include <vector>

class TomlConfigParser;

class Config final {
public:
        class WindowProperties final {
        private:
                friend class TomlConfigParser;
                
                inline static QSize   size;
                inline static QString title;

        public:
                static const QSize&   getSize();
                static const QString& getTitle();
        };

        class WindowLayoutProperties final {
        private:
                friend class TomlConfigParser;

                inline static std::vector<PrimaryButtonData> primary_power_buttons;
                
        public:
                static const std::vector<PrimaryButtonData>& getPrimaryPowerButtons();
        };

        class PrimaryButtonProperties final {
        private:
                friend class TomlConfigParser;

                inline static Qt::Alignment text_alignment;
                inline static Qt::Alignment icon_alignment;
                inline static QSize         icon_size;
                inline static QSizePolicy   policy;

        public:
                static const Qt::Alignment& getTextAlignment();
                static const Qt::Alignment& getIconAlignment();
                static const QSize&         getIconSize();
                static const QSizePolicy&   getPolicy();
        };

private:
        Config()                         = delete;
        Config(const Config&)            = delete;
        Config(Config&&)                 = delete;
        Config& operator=(const Config&) = delete;
        Config& operator=(Config&&)      = delete;
};
