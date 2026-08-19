// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/Type.h"
#include "Core/Config/Types/NodeView.h"
#include "Core/Config/Types/PathContext/PathContext.h"

#include <QStringView>
#include <cstddef>

// TODO: Consistent behaviors in chaining methods, separate overloads for ALL and index

namespace config {

// Stores a node with required metadata for use in Resolve method calls.
//
// 'node' contains the toml::node_view with data for extraction.
// 'type' is used for logging, to create QString path with config::PathContext.
// 'path_context' provides an extensible interface for convenient storage and extension of paths.
struct Candidate final {
        node_view    node;
        applet::Type applet;
        PathContext  path_context;

        // Creates Candidate copy extended by an additional segment.
        //
        // Replaces:
        //   Candidate new_cand = old_cand;
        //   new_cand.node = old_cand.node[key];
        //
        // With:
        //   auto new_cand = old_cand[key];
        Candidate operator[](QStringView key) const;

        // Creates Candidate copy extended by an additional index.
        //
        // Replaces:
        //   auto new_cand = old_cand;
        //   new_cand.node = old_cand.node[index];
        //
        // With:
        //   auto new_cand = old_cand[index];
        Candidate operator[](size_t index) const;
};

} // namespace config
