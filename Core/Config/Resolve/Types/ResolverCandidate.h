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

// TODO: Remove inlines for both; create dedicated Candidate.cpp
// TODO: Consistent behaviors in chaining methods, separate overloads for ALL and index
// TODO: Docs should explain what role these classes play in Resolve.h

namespace config::resolve {

// Stores a node with required metadata for use in Resolve method calls.
//
// 'node' contains the toml::node_view with data for extraction.
// 'type' is used for logging, to create QString path with config::resolve::PathContext.
// 'quiet' disables logging if true.
struct Candidate final {
        node_view    node;
        applet::type applet;
        bool         quiet = false;

        // Creates a copy of Candidate. An alternative to Candidate(old)
        // that makes the intention clearer when chaining.
        [[nodiscard]] Candidate makeCopy() const;

        // TODO: Consolidate repetitive logic
        // Extends CANDIDATE by KEY.
        //
        // Replaces:
        //   Candidate new_cand = old_cand;
        //   new_cand.node = old_cand.node[key];
        //
        // With:
        //   auto new_cand = old_cand.withExtension(key);
        [[nodiscard]] Candidate& withExtension(std::string_view key);

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

// Stores a dynamic array of Candidate objects for use in Resolve.
class Candidates final {
private:
        std::vector<Candidate> candidates;

public:
        Candidates() : candidates({}) {}
        Candidates(std::vector<Candidate> candidates) : candidates(std::move(candidates)) {}
        Candidates(std::initializer_list<Candidate> candidates) : candidates(candidates) {}

        [[nodiscard]] const std::vector<Candidate>& get() const { return candidates; }

        // Creates a copy of Candidates. An alternative to Candidates(old)
        // that makes the intention clearer when chaining.
        [[nodiscard]] Candidates makeCopy() const;

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
        [[nodiscard]] Candidates& withExtension(std::string_view key);

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
        [[nodiscard]] Candidates& withExtension(size_t index);

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
        [[nodiscard]] Candidates& withQuiet(bool quiet = true);

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
        [[nodiscard]] Candidates& withQuiet(std::optional<size_t> cand_index, bool quiet = true);
};

} // namespace config::resolve
