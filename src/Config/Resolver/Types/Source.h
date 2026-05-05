// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Config/Types/NodeView.h"

struct Source final {
        node_view        node;
        std::string_view scope;
        bool             quiet = false;
};
