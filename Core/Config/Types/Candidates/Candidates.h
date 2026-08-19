// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Core/Config/Types/Candidates/Candidate.h"

#include <QStringView>
#include <cstddef>
#include <initializer_list>
#include <utility>
#include <vector>

// TODO: Consistent behaviors in chaining methods, separate overloads for ALL and index

namespace config {

enum class CandidateIndex { Applet, Global };

// Stores a dynamic array of Candidate objects for use in Resolve.
class Candidates final {
private:
        std::vector<Candidate> candidates;

public:
        Candidates() : candidates({}) {}
        Candidates(std::vector<Candidate> candidates) : candidates(std::move(candidates)) {}
        Candidates(std::initializer_list<Candidate> candidates) : candidates(candidates) {}

        const Candidate& operator[](CandidateIndex i) const;

        [[nodiscard]] const std::vector<Candidate>& get() const { return candidates; }

        // Creates a copy of Candidates. An alternative to Candidates(old)
        // that makes the intention clearer when chaining.
        [[nodiscard]] Candidates makeCopy() const;

        // TODO: Consolidate repetitive logic
        // Creates Candidates copy extended by an additional segment.
        //
        // Replaces:
        //   auto new_cands = old_cands;
        //   for (size_t i = 0; i != new_cands.size(); ++i) {
        //       new_cands[i].candidates.node = old_cands[i].candidates.node[key];
        //   }
        //
        // With:
        //   auto new_cands = old_cands[key];
        Candidates operator[](QStringView key) const;

        // Creates Candidates copy extended by an additional index.
        //
        // Replaces:
        //   auto new_cands = old_cands;
        //   for (size_t i = 0; i != new_cands.size(); ++i) {
        //       new_cands[i].candidates.node = old_cands[i].candidates.node[key];
        //   }
        //
        // With:
        //   auto new_cands = old_cands[key];
        Candidates operator[](size_t index) const;
};

} // namespace config
