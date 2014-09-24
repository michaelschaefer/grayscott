#ifndef __FRAMEDISTRIBUTION_HH__
#define __FRAMEDISTRIBUTION_HH__


#include <cmath>
#include <ostream>
#include <vector>


typedef unsigned int uint;


class FrameDistribution : public std::vector<uint> {

public:

    typedef std::vector<uint>::iterator Iterator;
    typedef std::vector<uint>::const_iterator ConstIterator;

    enum Mode {
        Linear = 0,
        Logarithmic = 1,
        Quadratic = 2
    };

    static Mode modeFromInt(uint mode) {
        if (mode == 2) {
            return Quadratic;
        } else if (mode == 1) {
            return Logarithmic;
        } else {
            return Linear;
        }
    }

    FrameDistribution(uint timesteps, uint frames, Mode mode = Linear)
        : m_numberOfTimesteps(timesteps), m_numberOfFrames(frames),
          m_mode(mode)
    {
        double delta = 0;
        std::vector<uint> nonUniqueFrames;
        uint currentFrame = 0, oldFrame = 0;

        // # of frames cannot be bigger than # of timesteps
        if (m_numberOfFrames > m_numberOfTimesteps) {
            m_numberOfFrames = m_numberOfTimesteps;
        }

        if (m_mode == Linear) {
            delta = m_numberOfTimesteps / double(m_numberOfFrames);
            for (uint i = 0; i <= m_numberOfFrames; ++i) {
                nonUniqueFrames.push_back(uint(round(i*delta)));
            }
        } else if (m_mode == Logarithmic) {
            delta = log(m_numberOfTimesteps + 1) / m_numberOfFrames;
            for (uint i = 0; i <= m_numberOfFrames; ++i) {
                nonUniqueFrames.push_back(uint(round(exp(i*delta)-1)));
            }
        } else {
            delta = sqrt(m_numberOfTimesteps) / m_numberOfFrames;
            for (uint i = 0; i <= m_numberOfFrames; ++i) {
                nonUniqueFrames.push_back(uint(round(pow(i*delta, 2))));
            }
        }

        for (uint i = 0; i <= m_numberOfFrames; ++i) {
            currentFrame = nonUniqueFrames[i];

            if (i == 0) {
                oldFrame = currentFrame;
            } else {
                if (currentFrame <= oldFrame) {
                    oldFrame += 1;
                } else {
                    oldFrame = currentFrame;
                }
            }

            push_back(oldFrame);
        }
    }

    friend std::ostream& operator<<(std::ostream& stream,
                                    const FrameDistribution& stepper);


private:

    uint m_numberOfTimesteps;
    uint m_numberOfFrames;
    Mode m_mode;

};


#endif // __FRAMEDISTRIBUTION_HH__
