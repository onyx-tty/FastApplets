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

Candidate Candidate::makeCopy() const { return *this; }

Candidate& Candidate::withExtension(QStringView key) {
        // Qt logging uses UTF-16, TOML++ uses UTF-8
        auto byte_arr = key.toUtf8();

        node         = node[std::string_view(byte_arr.data(), byte_arr.size())];
        path_context = path_context.withExtension(key);

        return *this;
}

Candidate& Candidate::withExtension(size_t index) {
        node         = node[index];
        path_context = path_context.withExtension(index);

        return *this;
}
