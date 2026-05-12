// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <string>

// Stores absolute paths to config and keys
// TODO Rename to emphasize these are file paths
struct ConfigFiles final {
        std::string config;
        std::string keys;
};
