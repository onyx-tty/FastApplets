// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Candidates.h"
#include "Core/Config/Resolve/Types/Candidate.h"

#include <cstddef>
#include <optional>
#include <string_view>
#include <QDebug>
#include <QString>
#include <QStringView>
#include <QtGlobal>

using config::resolve::Candidate;
using config::resolve::Candidates;

const Candidate& Candidates::operator[](int i) const {
        return candidates[i];
}

Candidates Candidates::makeCopy() const {
        return *this;
}

Candidates& Candidates::withExtension(QStringView key) {
        // Qt logging uses UTF-16, TOML++ uses UTF-8
        auto byte_arr = key.toUtf8();

        for (auto& candidate : candidates) {
                candidate.node = candidate.node[std::string_view(byte_arr.data(), byte_arr.size())];
                candidate.path_context = candidate.path_context.withExtension(key);
        }

        return *this;
}

Candidates& Candidates::withExtension(size_t index) {
        for (auto& candidate : candidates) {
                candidate.node         = candidate.node[index];
                candidate.path_context = candidate.path_context.withExtension(index);
        }

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
                                      .arg(cand_index.value())
                                      .arg(candidates.size());
        }

        candidates[cand_index.value()].quiet = quiet;

        return *this;
}
