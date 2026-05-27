// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ResolverCandidate.h"

#include <cstddef>
#include <optional>
#include <string_view>
#include <utility>

/* Candidate */

ResolverCandidate ResolverCandidate::makeExtended(std::string_view key) const {
        ResolverCandidate new_candidate{};

        new_candidate.node   = node[key];
        new_candidate.applet = applet;
        new_candidate.quiet  = quiet;

        return new_candidate;
}

ResolverCandidate ResolverCandidate::makeExtended(size_t index) const {
        ResolverCandidate new_candidate{};

        new_candidate.node   = node[index];
        new_candidate.applet = applet;
        new_candidate.quiet  = quiet;

        return new_candidate;
}

ResolverCandidate ResolverCandidate::makeQuiet(bool quiet) const {
        ResolverCandidate new_candidate = *this;
        new_candidate.quiet             = quiet;

        return new_candidate;
}

/* Candidates */

ResolverCandidates ResolverCandidates::makeExtended(std::string_view key) const {
        ResolverCandidates new_candidates{};
        new_candidates.candidates.reserve(candidates.size());

        for (const auto& candidate : candidates) {
                new_candidates.candidates.push_back(candidate.makeExtended(key));
        }

        return std::move(new_candidates);
}

ResolverCandidates ResolverCandidates::makeExtended(size_t index) const {
        ResolverCandidates new_candidates{};
        new_candidates.candidates.reserve(candidates.size());

        for (const auto& candidate : candidates) {
                new_candidates.candidates.push_back(candidate.makeExtended(index));
        }

        return std::move(new_candidates);
}

ResolverCandidates ResolverCandidates::makeQuiet(bool                  quiet,
                                                 std::optional<size_t> cand_index) const {
        ResolverCandidates new_candidates = {};
        new_candidates.candidates.reserve(candidates.size());

        for (size_t i = 0; i != candidates.size(); ++i) {
                ResolverCandidate cand = candidates[i];

                // TODO: Simplify this chain
                if (!cand_index
                    || cand_index && cand_index.value() == i
                               && cand_index.value() < candidates.size()) {
                        cand.quiet = quiet;
                }

                new_candidates.candidates.push_back(cand);
        }

        return std::move(new_candidates);
}
