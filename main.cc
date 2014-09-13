#include "src/gui/grayscottwindow.hh"
#include "src/sim/keyframestepper.hh"
#include <QApplication>
#include <iostream>


using namespace std;


int main(int argc, char *argv[])
{    
    QApplication a(argc, argv);
    GrayScottWindow w;
    w.show();
    return a.exec();
}
