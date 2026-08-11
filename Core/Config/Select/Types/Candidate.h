// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/Type.h"
#include "Core/Config/Select/PathContext/PathContext.h"
#include "Core/Config/Types/NodeView.h"

#include <cstddef>
#include <QStringView>

// TODO: Consistent behaviors in chaining methods, separate overloads for ALL and index

namespace config::select {

// Stores a node with required metadata for use in Resolve method calls.
//
// 'node' contains the toml::node_view with data for extraction.
// 'type' is used for logging, to create QString path with config::select::PathContext.
// 'quiet' disables logging if true.
// 'path_context' provides an extensible interface for convenient storage and extension of paths.
struct Candidate final {
        node_view    node;
        applet::Type applet;
        bool         quiet = false;
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
        [[nodiscard]] Candidate& withExtension(QStringView key);

        // Extends CANDIDATE by INDEX.
        //
        // Replaces:
        //   auto new_cand = old_cand;
        //   new_cand.node = old_cand.node[index];
        //
        // With:
        //   auto new_cand = old_cand.withExtension(index);
        [[nodiscard]] Candidate& withExtension(size_t index);

        // Sets .quiet to QUIET.
        // True by default.
        //
        // Replaces:
        //   auto new_cand = old_cand;
        //   new_cand.quiet = true/false;
        //
        // With:
        //   auto new_cand = old_cand.withQuiet(true/false);
        [[nodiscard]] Candidate& withQuiet(bool quiet = true);
};

} // namespace config::select
