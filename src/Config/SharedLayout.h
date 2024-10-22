#ifndef SHARED_LAYOUT_H
#define SHARED_LAYOUT_H

#include <QIcon>
#include <QSize>
#include <QSizePolicy>

namespace icon {
extern const QSize size;
extern QIcon shutdown;
extern QIcon reboot;
extern QIcon suspend;
extern QIcon hibernate;
}

namespace policy {
extern const QSizePolicy buttons;
}

#endif // SHARED_LAYOUT_H
