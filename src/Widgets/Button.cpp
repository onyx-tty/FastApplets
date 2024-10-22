#include "Button.h"
#include "../Config/SharedLayout.h"

#include <QPushButton>
#include <QIcon>
#include <QVBoxLayout>

#include <QGridLayout>
#include <QLabel>

QPushButton* button::shutdown;
QPushButton* button::reboot;
QPushButton* button::suspend;
QPushButton* button::hibernate;

// Initializes a button of choice with uniform design
void button::init(QPushButton* button, QIcon button_icon) {
        button->setIcon(button_icon);
        button->setIconSize(icon::size);
        button->setSizePolicy(policy::buttons);
}

// Apply button::init to all buttons in the applet
void initButtons(QVBoxLayout* main_layout, QWidget* parent) {
        button::shutdown = new QPushButton(parent);
        button::init(button::shutdown, icon::shutdown);
        main_layout->addWidget(button::shutdown);
        debugAlignIconLeft(button::shutdown, "Shutdown");

        button::reboot = new QPushButton(parent);
        button::init(button::reboot, icon::reboot);
        main_layout->addWidget(button::reboot);
        debugAlignIconLeft(button::reboot, "Reboot");

        button::suspend = new QPushButton(parent);
        button::init(button::suspend, icon::suspend);
        main_layout->addWidget(button::suspend);
        debugAlignIconLeft(button::suspend, "Suspend");

        button::hibernate = new QPushButton(parent);
        button::init(button::hibernate, icon::hibernate);
        main_layout->addWidget(button::hibernate);
        debugAlignIconLeft(button::hibernate, "Hibernate");
}

void debugAlignIconLeft(QPushButton* button, QString label) { // Temporary solution, will require a QProxyStyle overwrite in the future
        QString style_sheet = "text-align: left;";
        button->setLayout(new QGridLayout);
        button->setStyleSheet(QString(style_sheet));
        QLabel* debug_text = new QLabel(label, button);
        debug_text->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter); // or center
        debug_text->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        button->layout()->addWidget(debug_text);
}
