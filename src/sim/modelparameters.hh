#ifndef __MODELPARAMETERS_HH__
#define __MODELPARAMETERS_HH__


#include <iostream>
#include <string>
#include "src/sim/keyframestepper.hh"


using namespace std;


typedef unsigned int uint;


/*
  Static class holding the parameters for the Gray-Scott model. The default
  values are:
  
  Du = 0.16  (diffusion of species u)
  Dv = 0.08  (diffusion of species v)
  F  = 0.035 (feed rate)
  k  = 0.06  (rate constant of second equation)
  N  = 256   (number of grid elements per dimension)
 */
class GrayScottModelParameters {

public:

    static void load(string filename);
    static void save(string filename);


    static bool export_to_file;

    static double Du;
    static double Dv;
    static double F;
    static double k;

    static KeyframeStepper::DistributionMode keyframeDistributionMode;

    static uint keyframes;
    static uint size;
    static uint timesteps;

};

#endif // __MODEL_HH__
