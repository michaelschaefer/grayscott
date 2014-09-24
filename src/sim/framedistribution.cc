#include "src/sim/framedistribution.hh"


std::ostream& operator<<(std::ostream& stream,
                         const FrameDistribution& stepper) {
    FrameDistribution::ConstIterator end = stepper.end();

    for (FrameDistribution::ConstIterator it = stepper.begin(); it != end; ++it) {
        if (it == stepper.begin()) {
            stream << *it;
        } else {
            stream << ", " << *it;
        }
    }

    return stream;
}
