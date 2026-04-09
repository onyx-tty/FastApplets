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

#include "Resolvers.h"
#include "Extractors.h"

#include <string_view>
#include <QString>

QString makeCfgPath(std::string_view scope, const QString& config_path, const char* separator) {
        return QString("in config.toml, %1%2%3").arg(scope, separator, config_path);
}

QString extendCfgPath(const QString& path, const char* extension, const char* separator) {
        if (path.isEmpty()) { return QString(extension); }

        return path + separator + extension;
}

QString makeKeysPath(std::string_view scope, const QString& keys_path, const char* separator) {
        return QString("in keys.toml, %1%2%3").arg(scope, separator, keys_path);
}

QString extendKeysPath(const QString& path, const char* extension, const char* separator) {
        if (path.isEmpty()) { return QString(extension); }

        return path + separator + extension;
}

std::optional<toml::array> resolve(std::initializer_list<Source> sources,
                                   const QString& path_context, bool force_override_on,
                                   const QString& error_arr_details, std::optional<size_t> min_size,
                                   std::optional<size_t> max_size) {
        // Validate and attempt extraction of each passed source, prioritizing earliest ones
        for (size_t i = 0; i != sources.size(); ++i) {
                auto& source = *(sources.begin() + i);

                // If 'i' is not the last index then 'source' is an override
                bool is_source_override = (i < sources.size() - 1) || force_override_on ? true
                                                                                        : false;

                if (auto result = extractor::array(source.node,
                                                   makeCfgPath(source.scope, path_context),
                                                   is_source_override, error_arr_details, min_size,
                                                   max_size)) {
                        return *result;
                }
        }

        // Use hardcoded defaults if extraction failed
        return std::nullopt;
}
