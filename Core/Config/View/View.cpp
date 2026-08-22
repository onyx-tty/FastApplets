// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: GPL-3.0-or-later

#include "View.h"
#include "Core/Config/Types/NodeView.h"

#include <QString>
#include <TomlQt/ArrayBounds.h>
#include <string_view>
#include <toml++/toml.hpp>
#include <utility>

using config::ConfigView;

namespace {}

ConfigView::ConfigView(node_view applet, node_view global, QString path, QString filename) :
        sources({applet, global}), path(std::move(path)), filename(std::move(filename)) {}

ConfigView ConfigView::operator[](std::string_view key) const {
        QString new_path = path;
        new_path.append(!new_path.isEmpty() ? "." : "");
        new_path.append(QString::fromUtf8(key));

        return {sources[0][key], sources[1][key], new_path, filename};
}

ConfigView ConfigView::operator[](int index) const {
        QString new_path = path;
        new_path.append(QString("[%1]").arg(index));

        return {sources[0][index], sources[1][index], new_path, filename};
}
