#ifndef __PARAMETERS_HH__
#define __PARAMETERS_HH__


#include <QString>
#include "src/sim/framedistribution.hh"


using namespace std;


typedef unsigned int uint;


namespace Parameters {

void load(string filename);
void save(string filename);


class Export {

public:

    static bool exportToFile;

    static FrameDistribution::Mode frameDistributionMode;

    static QString path;

    static uint frames;
    static uint timesteps;

};


class Model {

public:        

    static double Du;
    static double Dv;
    static double F;
    static double k;

    static uint size;
    static uint speedup;

};


}


#endif // __PARAMETERS_HH__
