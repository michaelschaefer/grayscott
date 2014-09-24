#ifndef GRAYSCOTTWINDOW_HH
#define GRAYSCOTTWINDOW_HH

#include <QtCore/Qt>
#include <QAction>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QKeySequence>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QVBoxLayout>
#include "src/gui/aboutdialog.hh"
#include "src/gui/buttonbox.hh"
#include "src/gui/parameterbox.hh"
#include "src/gui/simulationviewer.hh"
#include "src/gui/simulator.hh"
#include "src/gui/text.hh"
#include "src/sim/parameters.hh"
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

        QVBoxLayout* rightLayout = new QVBoxLayout();
        rightLayout->addWidget(m_buttonBox, 0, Qt::AlignVCenter);
        rightLayout->addWidget(m_simulationViewer, 0, Qt::AlignVCenter);

        QHBoxLayout* entireLayout = new QHBoxLayout();
        entireLayout->addWidget(m_parameterBox);
        entireLayout->addLayout(rightLayout);

        QWidget* panel = new QWidget(this);
        panel->setLayout(entireLayout);
        setCentralWidget(panel);

        createMenu();

        m_icon = QIcon(":/images/logo");
        setWindowIcon(m_icon);


        /*
         * simulator
         */

        m_simulator = new Simulator();
        QObject::connect(m_simulator, SIGNAL(statusUpdateable(QString)),
                         m_simulationViewer, SLOT(updateStatusLine(QString)));
        QObject::connect(m_simulator, SIGNAL(imageUpdateable(QImage, uint)),
                         m_simulationViewer, SLOT(updateImage(QImage, uint)));
        QObject::connect(m_simulator, SIGNAL(stopped()),
                         this, SLOT(simulationStopped()));
        QObject::connect(m_simulator, SIGNAL(exportDone()),
                         this, SLOT(simulationExportDone()));        
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
            if (m_parameterBox->saveParameters(m_simulationViewer) == true) {
                m_buttonBox->enableButton(Button::Pause | Button::Stop);
                m_simulator->start();
            }
        }

        if (button & Button::Stop) {
            m_buttonBox->enableButton(Button::Start);
            m_simulator->stop();
        }
    }

    void open() {
        QString filename = QFileDialog::getOpenFileName(this);
        if (filename.isNull() == false) {
            Parameters::load(filename.toStdString());
            m_parameterBox->loadParameters();
        }
    }

    void saveAs() {
        QString filename = QFileDialog::getSaveFileName(this);
        if (filename.isNull() == false) {
            m_parameterBox->saveParameters();
            Parameters::save(filename.toStdString());
        }
    }

    void showAbout(AboutDialog::Role role) {
        AboutDialog* dialog = new AboutDialog(this, role);
        dialog->exec();
    }

    inline void showAboutModel() { showAbout(AboutDialog::Model); }
    inline void showAboutProgram() { showAbout(AboutDialog::Program); }    

    void simulationExportDone() {
        m_simulator->pause();
        m_simulationViewer->disableExportToFile();
        QMessageBox::information(this, Text::Application::name(), Text::SimulationViewer::exportDoneText());
        m_simulator->resume();
    }

    void simulationStopped() {
        m_simulationViewer->updateStatusLine(Text::SimulationViewer::simulationStopped());
        m_buttonBox->enableButton(Button::Start);
    }

    void versionInformation() {
        QMessageBox about(this);
        about.setWindowTitle(Text::Help::versionInformationTitle());
        about.setText(Text::Help::versionInformationText());
        about.setDefaultButton(QMessageBox::Ok);
        about.setIconPixmap(windowIcon().pixmap(128, 128));
        about.show();
        about.exec();
    }


private:

    void createMenu() {
        QAction* action = 0;
        QMenu* menu = 0;


        /*
         * file menu
         */

        menu = new QMenu(Text::MenuBar::menuFile());

        action = new QAction(Text::MenuBar::actionOpen(), this);
        action->setShortcut(Qt::CTRL + Qt::Key_O);
        QObject::connect(action, SIGNAL(triggered()), this, SLOT(open()));
        menu->addAction(action);

        action = new QAction(Text::MenuBar::actionSaveAs(), this);
        action->setShortcut(Qt::CTRL + Qt::Key_S);
        QObject::connect(action, SIGNAL(triggered()), this, SLOT(saveAs()));
        menu->addAction(action);

        action = new QAction(Text::MenuBar::actionQuit(), this);
        action->setShortcut(Qt::CTRL + Qt::Key_Q);
        QObject::connect(action, SIGNAL(triggered()), this, SLOT(close()));
        menu->addSeparator();
        menu->addAction(action);

        menuBar()->addMenu(menu);


        /*
         * help menu
         */

        menu = new QMenu(Text::MenuBar::menuHelp());

        action = new QAction(Text::MenuBar::actionAboutModel(), this);
        QObject::connect(action, SIGNAL(triggered()),
                         this, SLOT(showAboutModel()));
        menu->addAction(action);

        action = new QAction(Text::MenuBar::actionAboutProgram(), this);
        QObject::connect(action, SIGNAL(triggered()),
                         this, SLOT(showAboutProgram()));
        menu->addAction(action);

        action = new QAction(Text::MenuBar::actionVersionInformation(), this);
        QObject::connect(action, SIGNAL(triggered()),
                         this, SLOT(versionInformation()));
        menu->addSeparator();
        menu->addAction(action);

        menuBar()->addMenu(menu);
    }    

    ButtonBox* m_buttonBox;
    QIcon m_icon;
    ParameterBox* m_parameterBox;
    SimulationViewer* m_simulationViewer;
    Simulator* m_simulator;

};

#endif // GRAYSCOTTWINDOW_HH
