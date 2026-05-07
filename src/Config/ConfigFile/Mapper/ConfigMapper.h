// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Config/Resolver/PathContext/PathContext.h"
#include "Config/Resolver/Resolver.h"
#include "Config/Types/NodePair.h"
#include "Config/Types/NodeView.h"

#include <cstddef>
#include <toml++/toml.hpp>
#include <QStringList>

class LayoutProperties;
class WindowProperties;
class GlobalConfig;
class PowerButtonParams;
class PrimaryButtonProperties;
class QString;
class QSize;
class QSizePolicy;

class ConfigMapper final {
private:
        /* Window Properties */
        static void mapWindow(NodePair nodes, WindowProperties& window,
                              const WindowProperties& defaults, const PathContext& path_context);

        /* Primary Button Properties */
        static void mapPrimaryButton(NodePair nodes, PrimaryButtonProperties& button,
                                     const PrimaryButtonProperties& defaults,
                                     const PathContext&             path_context);

        /* Layout Properties */
        static void mapLayout(node_view layout_node, LayoutProperties& layout,
                              const LayoutProperties& defaults, const PathContext& path_context);
        static void mapPrimaryButtons(node_view                             primary_buttons_node,
                                      std::vector<PowerButtonParams>&       primary_buttons,
                                      const std::vector<PowerButtonParams>& defaults,
                                      const PathContext&                    path_context);
        static bool mapPrimaryButton(node_view                             button_params_node,
                                     std::vector<PowerButtonParams>&       buttons,
                                     const std::vector<PowerButtonParams>& defaults,
                                     const PathContext&                    path_context);
        static void mapCommand(node_view command_node, std::vector<PowerButtonParams>& buttons,
                               const std::vector<PowerButtonParams>& defaults, QString& command,
                               const PathContext& path_context);

public:
        ConfigMapper() = delete;
        template<typename TConfig>
        static void mapToPowerAppletConfig(const toml::table& power_applet_table,
                                           const toml::table& global_table, TConfig& config);
};

#include "ConfigMapper.tpp"
