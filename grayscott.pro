#-------------------------------------------------
#
# Project created by QtCreator 2014-09-13T12:06:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

TARGET = grayscott
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11 -Ofast


SOURCES += \
    main.cc \
    src/sim/parameters.cc \
    src/sim/framedistribution.cc

HEADERS  += \
    src/gui/buttonbox.hh \
    src/gui/colortable.hh \
    src/gui/exportwidget.hh \
    src/gui/grayscottwindow.hh \
    src/gui/parameterbox.hh \
    src/gui/parameterslider.hh \
    src/gui/simulationviewer.hh \
    src/gui/simulator.hh \
    src/sim/colormap.hh \
    src/sim/exporter.hh \
    src/sim/model.hh \
    src/sim/squarematrix.hh \
    src/sim/parameters.hh \
    src/gui/text.hh \
    src/gui/framedistributionwidget.hh \
    src/sim/framedistribution.hh \
    src/gui/aboutdialog.hh

TRANSLATIONS += \
    src/res/grayscott_de.ts

RESOURCES += \
    src/res/resources.qrc
