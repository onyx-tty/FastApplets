#ifndef BUTTON_H
#define BUTTON_H

#include <QPushButton>
#include <QIcon>
#include <QVBoxLayout>

namespace button {
extern QPushButton* shutdown;
extern QPushButton* reboot;
extern QPushButton* suspend;
extern QPushButton* hibernate;

void init(QPushButton* button, QIcon icon);
}

void initButtons(QVBoxLayout* main_layout, QWidget* parent);

void debugAlignIconLeft(QPushButton* button, QString label); // Temporary solution, will require a QProxyStyle overwrite in the future

#endif // BUTTON_H
