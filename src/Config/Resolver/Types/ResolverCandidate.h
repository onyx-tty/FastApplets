// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Config/Types/NodeView.h"

#include <string_view>

// Stores a node with required metadata for use in Resolver method calls.
// node contains the toml::node_view with data for extraction.
// scope is used for logging, to create QString path with PathContext.
// quiet disables logging if true.
// TODO Replace scope with applet::type
struct ResolverCandidate final {
        node_view        node;
        std::string_view scope;
        bool             quiet = false;
};
