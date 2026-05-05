// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "NodeView.h"

struct NodePair final {
        node_view primary;
        node_view fallback;
};
