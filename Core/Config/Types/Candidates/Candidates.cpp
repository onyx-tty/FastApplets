// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Candidates.h"
#include "Core/Config/Types/Candidates/Candidate.h"

#include <QDebug>
#include <QString>
#include <QStringView>
#include <QtGlobal>
#include <cstddef>
#include <string_view>

using config::Candidate;
using config::Candidates;

const Candidate& Candidates::operator[](int i) const { return candidates[i]; }

const Candidate& Candidates::operator[](CandidateIndex i) const {
        return candidates[static_cast<size_t>(i)];
}

Candidates Candidates::makeCopy() const { return *this; }

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
