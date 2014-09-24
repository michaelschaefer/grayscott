#include "src/gui/grayscottwindow.hh"
#include <QApplication>
#include <QLibraryInfo>
#include <QLocale>
#include <QTranslator>


int main(int argc, char *argv[])
{
    // create application
    QApplication app(argc, argv);

    // allow locale switch via command line for debug purpose
    QString locale;
    if (argc == 1) {
        locale = QLocale::system().name();
    } else {
        locale = QString(argv[1]);
    }

    // translate Qt framework
    QString path = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + locale, path);
    app.installTranslator(&qtTranslator);

    // translate application
    QTranslator grayscottTranslator;
    grayscottTranslator.load(":/translations/grayscott_" + locale);
    app.installTranslator(&grayscottTranslator);

    // create main window and start application
    GrayScottWindow window;
    window.show();
    return app.exec();
}
