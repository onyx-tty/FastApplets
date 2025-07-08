#ifndef BUTTON_H
#define BUTTON_H

#include <QPushButton>
#include <QIcon>
#include <QVBoxLayout>
#include <QString>

class Button
        : public QPushButton {
        Q_OBJECT

protected:
        explicit Button(QWidget* parent,
                        QVBoxLayout* main_layout,
                        QIcon button_icon, // TODO default icon
                        QString text);

        virtual ~Button() = default;

public:
        // Workaround that aligns buttons to the left and keeps the text centered (sort of)
        /* It'll likely be removed once I get into actually inheriting from QProxyStyle
           It may be a bit wasteful, however as of now alternative solutions would clutter the code massively */
        void debugAlignIconLeft(QString label_text);
};

#endif // BUTTON_H
