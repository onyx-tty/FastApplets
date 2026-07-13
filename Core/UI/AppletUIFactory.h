// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

class MainWindow;
class Config;
class Keys;

// Initializes applet UI from scratch, including PrimaryButtons, CentralWidget, and MainWindow.
// MainWindow is constructed from provided config, keys, and default_keys.
MainWindow makeMainWindow(const Config& config, const Keys& keys, const Keys& default_keys);
