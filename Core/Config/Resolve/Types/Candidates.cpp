// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Candidates.h"
#include "Core/Config/Resolve/Types/Candidate.h"

#include <cstddef>
#include <optional>
#include <string_view>
#include <QDebug>
#include <QString>
#include <QtGlobal>

using config::resolve::Candidates;

Candidates Candidates::makeCopy() const {
        return *this;
}

Candidates& Candidates::withExtension(std::string_view key) {
        for (auto& candidate : candidates) { candidate.node = candidate.node[key]; }

        return *this;
}

Candidates& Candidates::withExtension(size_t index) {
        for (auto& candidate : candidates) { candidate.node = candidate.node[index]; }

        return *this;
}

Candidates& Candidates::withQuiet(bool quiet) {
        for (auto& candidate : candidates) { candidate.quiet = quiet; }

        return *this;
}

Candidates& Candidates::withQuiet(std::optional<size_t> cand_index, bool quiet) {
        if (!cand_index) { qWarning() << "Passed std::nullopt, cannot make anything quiet"; }

        if (cand_index.value() > candidates.size()) {
                qWarning() << QString("Passed index (%1) exceeds candidates.size() (%2)")
                                      .arg(cand_index.value(), candidates.size());
        }

        candidates[cand_index.value()].quiet = quiet;

        return *this;
}
