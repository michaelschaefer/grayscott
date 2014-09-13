#ifndef GRAYSCOTTWINDOW_HH
#define GRAYSCOTTWINDOW_HH

#include <QtCore/Qt>
#include <QAction>
#include <QFileDialog>
#include <QKeySequence>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QGridLayout>
#include "src/gui/buttonbox.hh"
#include "src/gui/help.hh"
#include "src/gui/parameterbox.hh"
#include "src/gui/simulationviewer.hh"
#include "src/gui/simulator.hh"
#include <iostream>


using namespace std;


class GrayScottWindow : public QMainWindow
{
    Q_OBJECT

public:

    GrayScottWindow() {
        /*
         * gui components
         */

        m_buttonBox = new ButtonBox(this);
        m_buttonBox->enableButton(Button::Start);
        QObject::connect(m_buttonBox, SIGNAL(clicked(Button)),
                         this, SLOT(clicked(Button)));

        m_parameterBox = new ParameterBox(this);
        m_simulationViewer = new SimulationViewer(this);

        QGridLayout* grid = new QGridLayout();
        grid->addWidget(m_parameterBox, 0, 0);
        grid->addWidget(m_buttonBox, 1, 0);
        grid->addWidget(m_simulationViewer, 0, 1, 2, 1, Qt::AlignVCenter);

        QWidget* panel = new QWidget(this);
        panel->setLayout(grid);
        setCentralWidget(panel);

        createMenu();

        setWindowIcon(QIcon("~/development/grayscottgui/logo.png"));


        /*
         * simulator
         */

        m_simulator = new Simulator();
        QObject::connect(m_simulator, SIGNAL(statusUpdateable(QString)),
                         m_simulationViewer, SLOT(updateStatusLine(QString)));
        QObject::connect(m_simulator, SIGNAL(imageUpdateable(QImage, uint)),
                         m_simulationViewer, SLOT(updateImage(QImage, uint)));
        QObject::connect(m_simulator, SIGNAL(done()),
                         this, SLOT(simulationDone()));
        QObject::connect(m_simulator, SIGNAL(terminated()),
                         this, SLOT(simulationTerminated()));
    }


private slots:

    void clicked(Button button) {
        if (button & Button::Pause) {
            m_buttonBox->enableButton(Button::Resume | Button::Stop);
            m_simulator->pause();
        }

        if (button & Button::Resume) {
            m_buttonBox->enableButton(Button::Pause | Button::Stop);
            m_simulator->resume();
        }

        if (button & Button::Start) {
            m_buttonBox->enableButton(Button::Pause | Button::Stop);
            m_parameterBox->forwardParameters(m_simulationViewer);
            m_simulator->start();
        }

        if (button & Button::Stop) {
            m_buttonBox->enableButton(Button::Start);
            m_simulator->terminate();
        }
    }

    void open() {
        QString filename = QFileDialog::getOpenFileName(this);
        if (filename.isNull() == false) {
            GrayScottModelParameters::load(filename.toStdString());
            m_parameterBox->loadParameters();
        }
    }

    void saveAs() {
        QString filename = QFileDialog::getSaveFileName(this);
        if (filename.isNull() == false) {
            m_parameterBox->forwardParameters();
            GrayScottModelParameters::save(filename.toStdString());
        }
    }

    void simulationDone() {
        m_simulationViewer->updateStatusLine("simulation finished");
        m_buttonBox->enableButton(Button::Start);
    }

    void simulationTerminated() {
        m_simulationViewer->updateStatusLine("simulation terminated");
        m_buttonBox->enableButton(Button::Start);
    }

    void versionInformation() {
        Help::showVersionInformation(this);
    }


private:

    void createMenu() {
        QAction* action = 0;
        QMenu* menu = 0;

        menu = new QMenu("&File");

        action = new QAction("&Open Configuration...", this);
        action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
        QObject::connect(action, SIGNAL(triggered()), this, SLOT(open()));
        menu->addAction(action);

        action = new QAction("&Save Configuration As...", this);
        action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
        QObject::connect(action, SIGNAL(triggered()), this, SLOT(saveAs()));
        menu->addAction(action);

        action = new QAction("&Quit", this);
        action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
        QObject::connect(action, SIGNAL(triggered()), this, SLOT(close()));
        menu->addSeparator();
        menu->addAction(action);

        menuBar()->addMenu(menu);

        menu = new QMenu("Help");

        action = new QAction("About Model", this);
        menu->addAction(action);

        action = new QAction("About Program", this);
        menu->addAction(action);

        action = new QAction("Version Information", this);
        QObject::connect(action, SIGNAL(triggered()),
                         this, SLOT(versionInformation()));
        menu->addSeparator();
        menu->addAction(action);

        menuBar()->addMenu(menu);
    }    

    ButtonBox* m_buttonBox;
    ParameterBox* m_parameterBox;
    SimulationViewer* m_simulationViewer;
    Simulator* m_simulator;

};

#endif // GRAYSCOTTWINDOW_HH
