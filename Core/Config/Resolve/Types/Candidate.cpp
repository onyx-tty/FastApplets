// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Candidate.h"

#include <cstddef>
#include <string_view>
#include <QDebug>
#include <QString>
#include <QtGlobal>

using config::resolve::Candidate;

Candidate Candidate::makeCopy() const {
        return *this;
}

Candidate& Candidate::withExtension(std::string_view key) {
        node         = node[key];
        path_context = path_context.withExtension(key);

        return *this;
}

Candidate& Candidate::withExtension(size_t index) {
        node         = node[index];
        path_context = path_context.withExtension(index);

        return *this;
}

Candidate& Candidate::withQuiet(bool quiet) {
        this->quiet = quiet;

        return *this;
}
