// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <toml++/toml.hpp>

// The full declaration is too long as a param!
// TOML nodes are read-only in this codebase, so const is fine.
using node_view = toml::node_view<const toml::node>;
