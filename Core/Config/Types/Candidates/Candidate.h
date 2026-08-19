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

        // Creates a copy of Candidate. An alternative to Candidate(old)
        // that makes the intention clearer when chaining.
        [[nodiscard]] Candidate makeCopy() const;

        // Extends CANDIDATE by KEY.
        //
        // Replaces:
        //   Candidate new_cand = old_cand;
        //   new_cand.node = old_cand.node[key];
        //
        // With:
        //   auto new_cand = old_cand.withExtension(key);
        Candidate& withExtension(QStringView key);

        // Extends CANDIDATE by INDEX.
        //
        // Replaces:
        //   auto new_cand = old_cand;
        //   new_cand.node = old_cand.node[index];
        //
        // With:
        //   auto new_cand = old_cand.withExtension(index);
        Candidate& withExtension(size_t index);
};

} // namespace config
