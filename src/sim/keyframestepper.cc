#include "keyframestepper.hh"


std::ostream& operator<<(std::ostream& stream,
                         const KeyframeStepper& stepper) {
    KeyframeStepper::ConstIterator end = stepper.end();

    for (KeyframeStepper::ConstIterator it = stepper.begin(); it != end; ++it) {
        if (it == stepper.begin()) {
            stream << *it;
        } else {
            stream << ", " << *it;
        }
    }

    return stream;
}
