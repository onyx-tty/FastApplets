#include "Button.h"
#include "../Config/SharedLayout.h"

#include <QGridLayout>
#include <QLabel>

/* Initializes a button of choice with uniform design */
// Inheriting constructor defaults from from QPushButton,
// but customizing the icon, icon size and the alignment of that button
Button::Button(QWidget* parent, // TODO default icon
               QVBoxLayout* main_layout,
               QIcon button_icon,
               QString text)
        : QPushButton(parent) {
        setIcon(button_icon);
        setIconSize(icon::size);
        setSizePolicy(policy::buttons);
        debugAlignIconLeft(text);
        main_layout->addWidget(this);
}

// Workaround that aligns buttons to the left and keeps the text centered (sort of)
/* It'll likely be removed once I get into actually inheriting from QProxyStyle
   It may be a bit wasteful, however as of now alternative solutions would clutter the code massively */
void Button::debugAlignIconLeft(QString label_text) {
        QString style_sheet = "text-align: left;";
        setLayout(new QGridLayout);
        setStyleSheet(QString(style_sheet));
        QLabel* debug_text = new QLabel(label_text, this);
        debug_text->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        debug_text->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        layout()->addWidget(debug_text);
}
