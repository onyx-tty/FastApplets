// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

class MainWindow;
class Config;
class Keys;

class AppletUIFactory final {
public:
        AppletUIFactory() = delete;

        static MainWindow make(const Config& config, const Keys& keys, const Keys& default_keys);
};
