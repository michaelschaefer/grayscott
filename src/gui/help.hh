#ifndef __HELP_HH__
#define __HELP_HH__


#include <QMessageBox>
#include <QString>
#include <QWidget>


class Help {

public:

    static void showAboutModel() {
    };

    static void showAboutProgram() {

    };

    static void showVersionInformation(QWidget* parent) {
        QString title = "About grayscottgui";
        QString text = "<b>grayscottgui 1.0</b><br/><br/>A program to simulate the Gray-Scott model for pattern formation.<br/><br/>Copyright (c) 2014 by Michael Schaefer (<a href=\"http://www.michael-schaefer.org/en/\">www.michael-schaefer.org/en/</a>)";
        QMessageBox::about(parent, title, text);
    };

};


#endif // __HELP_HH__
