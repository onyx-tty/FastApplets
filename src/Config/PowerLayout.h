#ifndef POWER_LAYOUT_H
#define POWER_LAYOUT_H

#include "SharedLayout.h"

#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <QIcon>

namespace icon {
extern const QSize size;
extern QString location;
extern QIcon shutdown;
extern QIcon reboot;
extern QIcon suspend;
extern QIcon hibernate;
}

namespace text {
extern QString shutdown;
extern QString reboot;
extern QString suspend;
extern QString hibernate;
}

namespace policy {
extern const QSizePolicy buttons; // expanding
}

#endif // POWER_LAYOUT_H
