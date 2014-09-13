#ifndef __SIMULATOR_HH__
#define __SIMULATOR_HH__


#include <QImage>
#include <QThread>
#include "src/sim/keyframestepper.hh"
#include "src/sim/model.hh"
#include "src/sim/modelparameters.hh"


typedef unsigned char uchar;
typedef unsigned int uint;

#include <iostream>
using namespace std;


class Simulator : public QThread {

    Q_OBJECT

signals:

    void done();
    void imageUpdateable(QImage, uint);
    void statusUpdateable(QString);
    void terminated();


public:

    Simulator()
        : QThread()
    {
        m_pause = false;
        m_pauseTime = 10;
        m_terminate = false;
    }

    void pause() {
        m_pause = true;
    }

    void resume() {
        m_pause = false;
    }

    void run() {
        uint keyframes = GrayScottModelParameters::keyframes;
        uint timesteps = GrayScottModelParameters::timesteps;
        KeyframeStepper::DistributionMode mode = GrayScottModelParameters::keyframeDistributionMode;
        KeyframeStepper stepper(timesteps, keyframes, mode);
        KeyframeStepper::Iterator keyframeIterator = stepper.begin();

        uint frameNumber = 0;
        uint progress;
        uint size = GrayScottModelParameters::size;

        GrayScottModel* problem = new GrayScottModel(size);
        problem->setDu(GrayScottModelParameters::Du);
        problem->setDv(GrayScottModelParameters::Dv);
        problem->setF(GrayScottModelParameters::F);
        problem->setk(GrayScottModelParameters::k);

        emitImageUpdateable(problem, frameNumber);
        if ((*keyframeIterator) == 0) {
            keyframeIterator++;
        }

        m_terminate = false;
        m_pause = false;        

        for (uint step=1; step < timesteps+1; ++step) {
            while (m_pause == true) {
                msleep(m_pauseTime);
            }

            if (m_terminate == true) {
                emit terminated();
                return;
            }

            problem->evolve();

            if (step == (*keyframeIterator)) {
                frameNumber++;
                emitImageUpdateable(problem, frameNumber);
                keyframeIterator++;                
            }

            progress = int(100 * (step + 1) / timesteps);
            emit statusUpdateable(QString().sprintf("%d%% finished", progress));
        }

        emit done();
    }

    void terminate() {
        m_pause = false;
        m_terminate = true;
    }


private:

    void emitImageUpdateable(GrayScottModel* problem, uint frameNumber) {
        uint size = problem->size();
        uchar* imageData = new uchar[size * size];
        SquareMatrix data = problem->v();
        data.scale(0, 255);
        data.toUChar(imageData);
        QImage image = QImage(imageData, size, size, QImage::Format_Indexed8);
        emit imageUpdateable(image, frameNumber);
    }

    bool m_pause;
    bool m_terminate;

    uint m_pauseTime;

};

#endif // __SIMULATOR_HH__
