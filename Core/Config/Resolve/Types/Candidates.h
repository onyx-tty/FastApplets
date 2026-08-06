// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/Resolve/Types/Candidate.h"

#include <cstddef>
#include <initializer_list>
#include <optional>
#include <utility>
#include <vector>
#include <QStringView>

// TODO: Consistent behaviors in chaining methods, separate overloads for ALL and index

namespace config::resolve {

// Stores a dynamic array of Candidate objects for use in Resolve.
class Candidates final {
private:
        std::vector<Candidate> candidates;

public:
        Candidates() : candidates({}) {}
        Candidates(std::vector<Candidate> candidates) : candidates(std::move(candidates)) {}
        Candidates(std::initializer_list<Candidate> candidates) : candidates(candidates) {}

        const Candidate& operator[](int i) const;

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
        [[nodiscard]] Candidates& withExtension(QStringView key);

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
