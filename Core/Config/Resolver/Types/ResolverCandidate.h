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

        // TODO: Consolidate repetitive logic
        // Return a new candidate with node extended by 'key'.
        //
        // Replaces:
        //   ResolverCandidate new_cand = old_cand;
        //   new_cand.node = old_cand.node[key];
        //
        // With:
        //   auto new_cand = old_cand.makeExtended(key);
        [[nodiscard]] ResolverCandidate makeExtended(std::string_view key) const;

        // Return a new candidate with node extended by 'index'.
        //
        // Replaces:
        //   auto new_cand = old_cand;
        //   new_cand.node = old_cand.node[index];
        //
        // With:
        //   auto new_cand = old_cand.makeExtended(index);
        [[nodiscard]] ResolverCandidate makeExtended(size_t index) const;

        // Returns a new candidate with quiet set to true/false.
        // Default is true.
        //
        // Replaces:
        //   auto new_cand = old_cand;
        //   new_cand.quiet = true/false;
        //
        // With:
        //   auto new_cand = old_cand.makeQuiet(true/false);
        [[nodiscard]] ResolverCandidate makeQuiet(bool quiet = true) const;
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

        // TODO: Consolidate repetitive logic
        // Returns a new dynamic array of candidates with ALL nodes extended by 'key'.
        //
        // Replaces:
        //   auto new_cands = old_cands;
        //   for (size_t i = 0; i != new_cands.size(); ++i) {
        //       new_cands[i].candidates.node = old_cands[i].candidates.node[key];
        //   }
        //
        // With:
        //   auto new_cands = old_cands.makeExtended(key);
        [[nodiscard]] ResolverCandidates makeExtended(std::string_view key) const;

        // Returns a new dynamic array of candidates with ALL nodes extended by 'key'.
        //
        // Replaces:
        //   auto new_cands = old_cands;
        //   for (size_t i = 0; i != new_cands.size(); ++i) {
        //       new_cands[i].candidates.node = old_cands[i].candidates.node[key];
        //   }
        //
        // With:
        //   auto new_cands = old_cands.makeExtended(key);
        [[nodiscard]] ResolverCandidates makeExtended(size_t index) const;

        // Returns a new dynamic array of candidates, all set to QUIET.
        //
        // Replaces:
        //   auto new_cands = old_cands;
        //   for (size_t i = 0; i != new_cands.size(); ++i) {
        //       new_cands[i].candidates.quiet = true/false;
        //   }
        //
        // With:
        //   auto new_cands = old_cands.makeQuiet(true/false);
        [[nodiscard]] ResolverCandidates makeQuiet(bool quiet = true) const;

        // Returns a new dynamic array of candidates, candidates[CAND_INDEX] set to QUIET.
        //
        // Replaces:
        //   auto new_cands = old_cands;
        //   for (size_t i = 0; i != new_cands.size(); ++i) {
        //       new_cands[i].candidates.quiet = true/false;
        //   }
        //
        // With:
        //   auto new_cands = old_cands.makeQuiet(true/false);
        [[nodiscard]] ResolverCandidates makeQuiet(std::optional<size_t> cand_index,
                                                   bool                  quiet = true) const;
};
