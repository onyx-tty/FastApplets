// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ResolverCandidate.h"
#include "CppUtils/Log/QtLog.h"

#include <cstddef>
#include <optional>
#include <string_view>
#include <utility>
#include <QString>

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

ResolverCandidates ResolverCandidates::makeQuiet(bool quiet) const {
        ResolverCandidates new_candidates = {};
        new_candidates.candidates.reserve(candidates.size());

        for (auto candidate : candidates) {
                new_candidates.candidates.push_back(candidate.makeQuiet(quiet));
        }

        return std::move(new_candidates);
}

ResolverCandidates ResolverCandidates::makeQuiet(std::optional<size_t> cand_index,
                                                 bool                  quiet) const {
        ResolverCandidates new_candidates = *this;
        new_candidates.candidates.reserve(candidates.size());

        if (!cand_index) { QWARNING() << "Passed std::nullopt, cannot make anything quiet!"; }

        if (cand_index.value() > candidates.size()) {
                QWARNING() << QString("Passed index (%1) exceeds candidates.size() (%2)")
                                      .arg(cand_index.value(), candidates.size());
        }

        new_candidates.candidates[cand_index.value()].quiet = quiet;

        return std::move(new_candidates);
}
