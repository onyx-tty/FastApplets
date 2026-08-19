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

const Candidate& Candidates::operator[](CandidateIndex i) const {
        return candidates[static_cast<int>(i)];
}

Candidates Candidates::operator[](QStringView key) const {
        // Qt logging uses UTF-16, TOML++ uses UTF-8
        auto byte_arr = key.toUtf8();

        Candidates copy = *this;

        for (size_t i = 0; i != candidates.size(); ++i) {
                copy.candidates[i].node =
                        candidates[i].node[std::string_view(byte_arr.data(), byte_arr.size())];
                copy.candidates[i].path_context = candidates[i].path_context[key];
        }

        return copy;
}

Candidates Candidates::operator[](size_t index) const {
        Candidates copy = *this;

        for (size_t i = 0; i != candidates.size(); ++i) {
                copy.candidates[i].node         = candidates[i].node[index];
                copy.candidates[i].path_context = candidates[i].path_context[index];
        }

        return copy;
}
