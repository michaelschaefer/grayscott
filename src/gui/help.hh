#ifndef __HELP_HH__
#define __HELP_HH__


#include <QIcon>
#include <QMessageBox>
#include <QString>
#include <QWidget>
#include "src/gui/text.hh"


class Help {

public:

    static void showAboutModel() {
    };

    static void showAboutProgram() {

    };

    static void showVersionInformation(QWidget* parent) {        
        QMessageBox about(parent);
        about.setWindowTitle(Text::Help::versionInformationTitle());
        about.setText(Text::Help::versionInformationText());
        about.setDefaultButton(QMessageBox::Ok);
        about.setIconPixmap(parent->windowIcon().pixmap(128, 128));
        about.show();
        about.exec();
    };

};


#endif // __HELP_HH__
