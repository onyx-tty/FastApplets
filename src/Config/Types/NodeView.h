// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "toml++/impl/node.hpp"
#include "toml++/impl/node_view.hpp"

using node_view = const toml::node_view<const toml::node>;
