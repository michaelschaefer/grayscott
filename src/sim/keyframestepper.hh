#ifndef __KEYFRAMESTEPPER_HH__
#define __KEYFRAMESTEPPER_HH__


#include <cmath>
#include <ostream>
#include <vector>


typedef unsigned int uint;


class KeyframeStepper : public std::vector<uint> {

public:

    typedef std::vector<uint>::iterator Iterator;
    typedef std::vector<uint>::const_iterator ConstIterator;

    enum DistributionMode {
        Linear = 0,
        Logarithmic = 1,
        Squared = 2
    };

    KeyframeStepper(uint timesteps, uint keyframes,
                    DistributionMode mode = Linear)
        : m_numberOfTimesteps(timesteps), m_numberOfKeyframes(keyframes),
          m_distributionMode(mode)
    {
        double delta = 0;
        std::vector<uint> nonUniqueKeyframes;
        uint currentKeyframe = 0, oldKeyframe = 0;

        // # of keyframes cannot be bigger than # of timesteps
        if (m_numberOfKeyframes > m_numberOfTimesteps) {
            m_numberOfKeyframes = m_numberOfTimesteps;
        }

        if (m_distributionMode == Linear) {
            delta = m_numberOfTimesteps / double(m_numberOfKeyframes);
            for (uint i = 0; i <= m_numberOfKeyframes; ++i) {
                nonUniqueKeyframes.push_back(uint(round(i*delta)));
            }
        } else if (m_distributionMode == Logarithmic) {
            delta = log(m_numberOfTimesteps + 1) / m_numberOfKeyframes;
            for (uint i = 0; i <= m_numberOfKeyframes; ++i) {
                nonUniqueKeyframes.push_back(uint(round(exp(i*delta)-1)));
            }

        } else {
            delta = sqrt(m_numberOfTimesteps) / m_numberOfKeyframes;
            for (uint i = 0; i <= m_numberOfKeyframes; ++i) {
                nonUniqueKeyframes.push_back(uint(round(pow(i*delta, 2))));
            }
        }

        for (uint i = 0; i <= m_numberOfKeyframes; ++i) {
            currentKeyframe = nonUniqueKeyframes[i];

            if (i == 0) {
                oldKeyframe = currentKeyframe;
            } else {
                if (currentKeyframe <= oldKeyframe) {
                    oldKeyframe += 1;                    
                } else {
                    oldKeyframe = currentKeyframe;
                }
            }

            this->push_back(oldKeyframe);
        }
    }

    friend std::ostream& operator<<(std::ostream& stream,
                                    const KeyframeStepper& stepper);


private:

    uint m_numberOfTimesteps;
    uint m_numberOfKeyframes;
    DistributionMode m_distributionMode;

};


#endif // __KEYFRAMESTEPPER_HH__
