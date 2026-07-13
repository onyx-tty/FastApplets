// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ResolverCandidate.h"

#include <cstddef>
#include <optional>
#include <string_view>
#include <QDebug>
#include <QString>
#include <QtGlobal>

/* Candidate */

config::resolve::Candidate config::resolve::Candidate::makeCopy() const {
        return *this;
}

config::resolve::Candidate& config::resolve::Candidate::withExtension(std::string_view key) {
        node = node[key];

        return *this;
}

config::resolve::Candidate& config::resolve::Candidate::withExtension(size_t index) {
        node = node[index];

        return *this;
}

config::resolve::Candidate& config::resolve::Candidate::withQuiet(bool quiet) {
        this->quiet = quiet;

        return *this;
}

/* Candidates */

config::resolve::Candidates config::resolve::Candidates::makeCopy() const {
        return *this;
}

config::resolve::Candidates& config::resolve::Candidates::withExtension(std::string_view key) {
        for (auto& candidate : candidates) { candidate.node = candidate.node[key]; }

        return *this;
}

config::resolve::Candidates& config::resolve::Candidates::withExtension(size_t index) {
        for (auto& candidate : candidates) { candidate.node = candidate.node[index]; }

        return *this;
}

config::resolve::Candidates& config::resolve::Candidates::withQuiet(bool quiet) {
        for (auto& candidate : candidates) { candidate.quiet = quiet; }

        return *this;
}

config::resolve::Candidates& config::resolve::Candidates::withQuiet(std::optional<size_t> cand_index,
                                                                    bool quiet) {
        if (!cand_index) { qWarning() << "Passed std::nullopt, cannot make anything quiet!"; }

        if (cand_index.value() > candidates.size()) {
                qWarning() << QString("Passed index (%1) exceeds candidates.size() (%2)")
                                      .arg(cand_index.value(), candidates.size());
        }

        candidates[cand_index.value()].quiet = quiet;

        return *this;
}
