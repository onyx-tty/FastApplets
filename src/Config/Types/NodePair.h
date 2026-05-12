// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "NodeView.h"

// Holds two node views for priority-based value extraction.
//
// Usage pattern:
//   1. Extract value from primary node
//   2. If primary node is missing/invalid, extract from the fallback node
//   3. If both missing, use hardcoded defaults (stored elsewhere)
//
// This struct does NOT store defaults - that's the caller's responsibility.
//
// Names reflect extraction priority (primary first), but fallback is the
// only other source, there is no secondary, tertiary, etc.
//
// TODO Replace this struct with something else, NodePair is a confusing name,
//      and ideally there should be no limit to how many "fallbacks" you can pass.
//      Fallback itself is not a good name, it does not emphasize priorities.
struct NodePair final {
        node_view primary;  // First source to try for extraction
        node_view fallback; // Fallback source if primary is missing or invalid
};
