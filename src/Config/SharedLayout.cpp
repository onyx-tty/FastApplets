#include "SharedLayout.h"

#include <QIcon>
#include <QSize>
#include <QSizePolicy>
#include <QString>

QString icon_location = "../Applets/PowerApplet/Data/Icons/";

const QSize icon::size(64, 64);
QIcon icon::shutdown(icon_location + "shutdown.svg");
QIcon icon::reboot(icon_location + "reboot.svg");
QIcon icon::suspend(icon_location + "suspend.svg");
QIcon icon::hibernate(icon_location + "hibernate.svg");

const QSizePolicy policy::buttons(QSizePolicy::Expanding, QSizePolicy::Expanding);
