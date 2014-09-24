#ifndef __TEXT_HH__
#define __TEXT_HH__


#include <QString>
#include <QWidget>


namespace Text {


namespace Application {

static QString about() {
    return QWidget::trUtf8(
                "A program to simulation the Gray-Scott model for "
                "pattern formation"
                );
}
static QString copyright() {
    return QWidget::trUtf8(
                "Copyright (c) 2014 Michael Schaefer "
                "(<a href=\"http://www.michael-schaefer.org/en/\">"
                "www.michael-schaefer.org/en/</a>)"
                );
}
static QString name() { return QWidget::trUtf8("grayscott"); }
static QString version() { return QWidget::trUtf8("1.0"); }

}


namespace Export {

static QString directoryButton() { return QWidget::trUtf8("Choose directory"); }
static QString directoryPlaceholder() { return QWidget::trUtf8("none selected"); }
static QString frames() { return QWidget::trUtf8("frames"); }
static QString groupTitle() { return QWidget::trUtf8("Export frames"); }
static QString messageNoPathText() { return QWidget::trUtf8("Please select an export path!"); }
static QString timesteps() { return QWidget::trUtf8("timesteps"); }

}


namespace FrameDistribution {

static QString groupTitle() { return QWidget::trUtf8("Frame distribution"); }
static QString linear() { return QWidget::trUtf8("linear"); }
static QString logarithmic() { return QWidget::trUtf8("logarithmic"); }
static QString quadratic() { return QWidget::trUtf8("quadratic"); }

}


namespace Help {

static QString versionInformationTitle() {
    return QWidget::trUtf8("About grayscott");
}
static QString versionInformationText() {
    return QString(
                "<b>" + Application::name() + " " + Application::version() +
                "</b><br/><br/>" + Application::about() + "<br/><br/>" +
                Application::copyright()
                );
}

}


namespace MenuBar {

static QString menuFile() { return QWidget::trUtf8("&File"); }
static QString actionOpen() { return QWidget::trUtf8("&Open configuration..."); }
static QString actionQuit() { return QWidget::trUtf8("&Quit"); }
static QString actionSaveAs() { return QWidget::trUtf8("&Save configuration as..."); }

static QString menuHelp() { return QWidget::trUtf8("&Help"); }
static QString actionAboutModel() { return QWidget::trUtf8("About the &model..."); }
static QString actionAboutProgram() { return QWidget::trUtf8("About the &program..."); }
static QString actionVersionInformation() { return QWidget::trUtf8("&Version information..."); }

}


namespace ParameterBox {

namespace Rendering {

static QString fast() { return QWidget::trUtf8("fast"); }
static QString groupTitle() { return QWidget::trUtf8("Visualization parameters"); }
static QString groupSpeedTitle() { return QWidget::trUtf8("rendering speed"); }
static QString normal() { return QWidget::trUtf8("normal"); }
static QString slow() { return QWidget::trUtf8("slow"); }
static QString veryFast() { return QWidget::trUtf8("very fast"); }

}

namespace System {

static QString bacteria1() { return QWidget::trUtf8("bacteria1"); }
static QString groupTitle() { return QWidget::trUtf8("System parameters"); }
static QString size() { return QWidget::trUtf8("size"); }
static QString unstable() { return QWidget::trUtf8("unstable"); }
static QString userDefined() { return QWidget::trUtf8("<user-defined>"); }
static QString zebra() { return QWidget::trUtf8("zebra"); }

}

}


namespace SimulationControl {

static QString pause() { return QWidget::trUtf8("pause"); }
static QString resume() { return QWidget::trUtf8("resume"); }
static QString start() { return QWidget::trUtf8("start"); }
static QString stop() { return QWidget::trUtf8("stop"); }

}


namespace SimulationViewer {

static QString exportDoneText() { return QWidget::trUtf8("Export finished! Simulation will continue without writing further images to disk."); }
static QString noSimulationRunning() { return QWidget::trUtf8("No simulation running"); }
static QString simulationStopped() { return QWidget::trUtf8("Simulation stopped"); }
static QString statusExport() { return QWidget::trUtf8("Export finished to %1%"); }
static QString statusNoExport() { return QWidget::trUtf8("%1 frames rendered"); }

}

}


#endif // __TEXT_HH__
