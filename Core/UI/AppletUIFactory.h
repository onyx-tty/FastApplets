// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

class MainWindow;
class Config;
class Keys;

// Builds applet UI from scratch, including PrimaryButtons, CentralWidget, and MainWindow.
class AppletUIFactory final {
public:
        AppletUIFactory() = delete;

        // Builds MainWindow from provided config, keys, and default_keys.
        static MainWindow make(const Config& config, const Keys& keys, const Keys& default_keys);
};
