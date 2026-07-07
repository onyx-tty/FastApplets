// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Applets/Types/AppletType.h"
#include "Core/Config/Types/NodeView.h"

#include <cstddef>
#include <initializer_list>
#include <optional>
#include <string_view>
#include <utility>
#include <vector>

// TODO: Remove inlines for both; create dedicated ResolverCandidate.cpp
// TODO: Consistent behaviors in chaining methods, separate overloads for ALL and index
// TODO: Docs should explain what role these classes play in Resolver.h

// Stores a node with required metadata for use in Resolver method calls.
//
// 'node' contains the toml::node_view with data for extraction.
// 'type' is used for logging, to create QString path with PathContext.
// 'quiet' disables logging if true.
struct ResolverCandidate final {
        node_view    node;
        applet::type applet;
        bool         quiet = false;

        // Creates a copy of ResolverCandidate. An alternative to ResolverCandidate(old)
        // that makes the intention clearer when chaining.
        [[nodiscard]] ResolverCandidate makeCopy() const;

        // TODO: Consolidate repetitive logic
        // Extends CANDIDATE by KEY.
        //
        // Replaces:
        //   ResolverCandidate new_cand = old_cand;
        //   new_cand.node = old_cand.node[key];
        //
        // With:
        //   auto new_cand = old_cand.withExtension(key);
        [[nodiscard]] ResolverCandidate& withExtension(std::string_view key);

        // Extends CANDIDATE by INDEX.
        //
        // Replaces:
        //   auto new_cand = old_cand;
        //   new_cand.node = old_cand.node[index];
        //
        // With:
        //   auto new_cand = old_cand.withExtension(index);
        [[nodiscard]] ResolverCandidate& withExtension(size_t index);

        // Sets .quiet to QUIET.
        // True by default.
        //
        // Replaces:
        //   auto new_cand = old_cand;
        //   new_cand.quiet = true/false;
        //
        // With:
        //   auto new_cand = old_cand.withQuiet(true/false);
        [[nodiscard]] ResolverCandidate& withQuiet(bool quiet = true);
};

// Stores a dynamic array of ResolverCandidate objects for use in Resolver.
class ResolverCandidates final {
private:
        std::vector<ResolverCandidate> candidates;

public:
        ResolverCandidates() : candidates({}) {}
        ResolverCandidates(std::vector<ResolverCandidate> candidates) :
                candidates(std::move(candidates)) {}
        ResolverCandidates(std::initializer_list<ResolverCandidate> candidates) :
                candidates(candidates) {}

        [[nodiscard]] const std::vector<ResolverCandidate>& get() const { return candidates; }

        // Creates a copy of ResolverCandidates. An alternative to ResolverCandidates(old)
        // that makes the intention clearer when chaining.
        [[nodiscard]] ResolverCandidates makeCopy() const;

        // TODO: Consolidate repetitive logic
        // Extends ALL CANDIDATES by KEY.
        //
        // Replaces:
        //   auto new_cands = old_cands;
        //   for (size_t i = 0; i != new_cands.size(); ++i) {
        //       new_cands[i].candidates.node = old_cands[i].candidates.node[key];
        //   }
        //
        // With:
        //   auto new_cands = old_cands.withExtension(key);
        [[nodiscard]] ResolverCandidates& withExtension(std::string_view key);

        // Extends ALL CANDIDATES by KEY.
        //
        // Replaces:
        //   auto new_cands = old_cands;
        //   for (size_t i = 0; i != new_cands.size(); ++i) {
        //       new_cands[i].candidates.node = old_cands[i].candidates.node[key];
        //   }
        //
        // With:
        //   auto new_cands = old_cands.withExtension(key);
        [[nodiscard]] ResolverCandidates& withExtension(size_t index);

        // Sets .quiet in ALL CANDIDATES to QUIET.
        //
        // Replaces:
        //   auto new_cands = old_cands;
        //   for (size_t i = 0; i != new_cands.size(); ++i) {
        //       new_cands[i].candidates.quiet = true/false;
        //   }
        //
        // With:
        //   auto new_cands = old_cands.withQuiet(true/false);
        [[nodiscard]] ResolverCandidates& withQuiet(bool quiet = true);

        // Sets .quiet in CANDIDATES[CAND_INDEX] to QUIET.
        //
        // Replaces:
        //   auto new_cands = old_cands;
        //   for (size_t i = 0; i != new_cands.size(); ++i) {
        //       new_cands[i].candidates.quiet = true/false;
        //   }
        //
        // With:
        //   auto new_cands = old_cands.withQuiet(index, true/false);
        [[nodiscard]] ResolverCandidates& withQuiet(std::optional<size_t> cand_index,
                                                    bool                  quiet = true);
};
