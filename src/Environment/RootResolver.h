/* Pretty Applets

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

#include <QString>

class RootResolver {
private:
        QString project_root_marker;
        QString project_root;

        explicit RootResolver(const QString& project_root_marker);
public:
        RootResolver(const RootResolver&) = delete;
        RootResolver(RootResolver&&) = delete;
        RootResolver& operator=(const RootResolver&) = delete;
        RootResolver& operator=(RootResolver&&) = delete;

        static RootResolver& getInstance();
        const QString& getProjectRoot();
};