// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Applets/Types/AppletType.h"
#include "Config/Types/NodeView.h"

#include <cstddef>
#include <initializer_list>
#include <string_view>
#include <utility>
#include <vector>

// TODO: Remove inlines for both; create dedicated ResolverCandidate.cpp
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
        //   ResolverCandidate new_cand{} = old_cand;
        //   new_cand.node = old_cand.node[key];
        //
        // With:
        //   auto new_cand = old_cand.makeExtended(key);
        inline ResolverCandidate makeExtended(std::string_view key) const {
                ResolverCandidate new_candidate{};

                new_candidate.node   = node[key];
                new_candidate.applet = applet;
                new_candidate.quiet  = quiet;

                return new_candidate;
        }

        // Return a new candidate with node extended by 'index'.
        //
        // Replaces:
        //   auto new_cand{} = old_cand;
        //   new_cand.node = old_cand.node[index];
        //
        // With:
        //   auto new_cand = old_cand.makeExtended(index);
        inline ResolverCandidate makeExtended(size_t index) const {
                ResolverCandidate new_candidate{};

                new_candidate.node   = node[index];
                new_candidate.applet = applet;
                new_candidate.quiet  = quiet;

                return new_candidate;
        }
};

// Stores a dynamic array of ResolverCandidate objects for use in Resolver.
//
// 'node' contains the toml::node_view with data for extraction.
// 'type' is used for logging, to create QString path with PathContext.
// 'quiet' disables logging if true.
class ResolverCandidates final {
private:
        std::vector<ResolverCandidate> candidates;

public:
        ResolverCandidates() : candidates({}) {}
        ResolverCandidates(std::vector<ResolverCandidate> candidates) :
                candidates(std::move(candidates)) {}
        ResolverCandidates(std::initializer_list<ResolverCandidate> candidates) :
                candidates(candidates) {}

        [[nodiscard]] inline const std::vector<ResolverCandidate>& get() const {
                return candidates;
        }

        // TODO: Consolidate repetitive logic
        // Return a new dynamic array of candidates with ALL nodes extended by 'key'.
        //
        // Replaces:
        //   auto new_cands{} = old_cands;
        //   for (size_t i = 0; i != new_cands.size(); ++i) {
        //       new_cands[i].candidates.node = old_cands[i].candidates.node[key];
        //   }
        //
        // With:
        //   auto new_cands = old_cands.makeExtended(key);
        [[nodiscard]] inline ResolverCandidates makeExtended(std::string_view key) const {
                ResolverCandidates new_candidates{};
                new_candidates.candidates.reserve(candidates.size());

                for (const auto& candidate : candidates) {
                        new_candidates.candidates.push_back(candidate.makeExtended(key));
                }

                return std::move(new_candidates);
        }

        // Return a new dynamic array of candidates with ALL nodes extended by 'key'.
        //
        // Replaces:
        //   auto new_cands{} = old_cands;
        //   for (size_t i = 0; i != new_cands.size(); ++i) {
        //       new_cands[i].candidates.node = old_cands[i].candidates.node[key];
        //   }
        //
        // With:
        //   auto new_cands = old_cands.makeExtended(key);
        [[nodiscard]] inline ResolverCandidates makeExtended(size_t index) const {
                ResolverCandidates new_candidates{};
                new_candidates.candidates.reserve(candidates.size());

                for (const auto& candidate : candidates) {
                        new_candidates.candidates.push_back(candidate.makeExtended(index));
                }

                return std::move(new_candidates);
        }
};
