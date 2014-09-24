#ifndef __SIMULATOR_HH__
#define __SIMULATOR_HH__


#include <QMessageBox>
#include <QImage>
#include <QThread>
#include <QElapsedTimer>
#include "src/gui/text.hh"
#include "src/sim/framedistribution.hh"
#include "src/sim/model.hh"
#include "src/sim/parameters.hh"


typedef unsigned char uchar;
typedef unsigned int uint;

#include <iostream>
using namespace std;


class Simulator : public QThread {

    Q_OBJECT

signals:

    void exportDone();
    void imageUpdateable(QImage, uint);
    void statusUpdateable(QString);
    void stopped();


public:

    Simulator()
        : QThread()
    {
        m_pause = false;
        m_pauseTime = 10;
        m_stop = false;
    }

    void pause() {
        m_pause = true;
    }

    void resume() {
        m_pause = false;
    }

    void run() {
        /*
         * set local variables
         */

        bool exportToFile = Parameters::Export::exportToFile;

        uint frameNumber = 0;
        uint frames = Parameters::Export::frames;
        uint progress;
        uint size = Parameters::Model::size;
        uint step = 0;
        uint timesteps = Parameters::Export::timesteps;

        GrayScottModel* problem = new GrayScottModel(size);
        problem->setDu(Parameters::Model::Du);
        problem->setDv(Parameters::Model::Dv);
        problem->setF(Parameters::Model::F);
        problem->setk(Parameters::Model::k);

        FrameDistribution::Mode mode = Parameters::Export::frameDistributionMode;
        FrameDistribution distribution(timesteps, frames, mode);
        FrameDistribution::Iterator frameDistributionIterator = distribution.begin();

        QString msg;


        // show initial values and advance keyframe iterator
        emitImageUpdateable(problem, frameNumber);
        if ((*frameDistributionIterator) == 0) {
            frameDistributionIterator++;
        }

        m_pause = false;
        m_stop = false;

        // start timers
        m_calculationLimitTimer.start();
        m_frameLimitTimer.start();


        /*
         * evolution loop
         */
        while (true) {
            step++;

            while (m_pause == true) {
                msleep(m_pauseTime);
            }

            if (m_stop == true) {
                break;
            }

            waitForNextCalculation();
            problem->evolve();


            /*
             * gui update
             */

            if (exportToFile == true) {
                progress = (100 * step) / timesteps;
                msg = Text::SimulationViewer::statusExport().arg(progress);

                if (step == (*frameDistributionIterator)) {
                    frameNumber++;
                    emitImageUpdateable(problem, frameNumber);

                    frameDistributionIterator++;
                    if (frameDistributionIterator == distribution.end()) {
                        pause();
                        exportToFile = false;
                        emit statusUpdateable(Text::SimulationViewer::statusExport().arg(100));
                        emit exportDone();
                        continue;
                    }
                }
            } else {
                msg = Text::SimulationViewer::statusNoExport().arg(step);

                if (readyForNextFrame() == true) {
                    emitImageUpdateable(problem);
                }
            }

            emit statusUpdateable(msg);
        }

        emit stopped();
    }

    void stop() {
        m_pause = false;
        m_stop = true;
    }


private:

    void emitImageUpdateable(GrayScottModel* problem, uint frameNumber=0) {
        uint size = problem->size();
        uchar* imageData = new uchar[size * size];
        SquareMatrix data = problem->v();
        data.scale(0, 255);
        data.toUChar(imageData);
        QImage image = QImage(imageData, size, size, QImage::Format_Indexed8);
        emit imageUpdateable(image, frameNumber);
    }

    void guiUpdate() {

    }

    bool readyForNextFrame() {
        if (m_frameLimitTimer.elapsed() >= 20) {
            m_frameLimitTimer.restart();
            return true;
        } else {
            return false;
        }
    }

    void waitForNextCalculation() {
        if (Parameters::Model::speedup == 0) {
            return;
        }

        qint64 goal = 1000 * Parameters::Model::speedup;
        qint64 delta = goal - m_calculationLimitTimer.nsecsElapsed() / 1000;
        if (delta > 0) {
            usleep(delta);
        }
        m_calculationLimitTimer.restart();
    }


    bool m_pause;
    bool m_stop;

    QElapsedTimer m_calculationLimitTimer;
    QElapsedTimer m_frameLimitTimer;

    uint m_pauseTime;

};

#endif // __SIMULATOR_HH__
