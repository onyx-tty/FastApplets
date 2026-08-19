// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Candidate.h"

#include <QDebug>
#include <QString>
#include <QStringView>
#include <QtGlobal>
#include <cstddef>
#include <string_view>

using config::Candidate;

Candidate Candidate::operator[](QStringView key) const {
        // Qt logging uses UTF-16, TOML++ uses UTF-8
        auto byte_arr = key.toUtf8();

        Candidate copy = *this;

        copy.node         = node[std::string_view(byte_arr.data(), byte_arr.size())];
        copy.path_context = path_context[key];

        return copy;
}

Candidate Candidate::operator[](size_t index) const {
        Candidate copy = *this;

        copy.node         = node[index];
        copy.path_context = path_context[index];

        return copy;
}
