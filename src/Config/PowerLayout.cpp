#include "PowerLayout.h"

// Modify these as much as you want to customize the appearance

const QString icon_location = "Data/";

// icon
const QSize icon::size = shared_icon::size;
QString icon::location(icon_location);
QIcon icon::shutdown(icon_location + "shutdown.svg");
QIcon icon::reboot(icon_location + "reboot.svg");
QIcon icon::suspend(icon_location + "suspend.svg");
QIcon icon::hibernate(icon_location + "hibernate.svg");

// text
QString text::shutdown("Shutdown");
QString text::reboot("Reboot");
QString text::suspend("Suspend");
QString text::hibernate("Hibernate");

// policy
const QSizePolicy policy::buttons = shared_policy::buttons;
