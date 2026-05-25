// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Applets/Types/AppletType.h"
#include "Config/Types/NodeView.h"

// Stores a node with required metadata for use in Resolver method calls.
// node contains the toml::node_view with data for extraction.
// type is used for logging, to create QString path with PathContext.
// quiet disables logging if true.
// TODO Replace scope with applet::type
struct ResolverCandidate final {
        node_view    node;
        applet::type applet;
        bool         quiet = false;
};
