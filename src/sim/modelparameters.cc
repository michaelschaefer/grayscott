#include <fstream>
#include <iostream>
#include "src/sim/modelparameters.hh"

void GrayScottModelParameters::load(string filename) {
    ifstream file(filename, ios_base::in | ios_base::binary);
    file >> Du;
    file >> Dv;
    file >> F;
    file >> k;
    file >> keyframes;
    file >> size;
    file >> timesteps;
    file.close();
}

void GrayScottModelParameters::save(string filename) {
    ofstream file(filename, ios_base::out | ios_base::binary);
    file << Du << endl;
    file << Dv << endl;
    file << F << endl;
    file << k << endl;
    file << keyframes << endl;
    file << size << endl;
    file << timesteps << endl;
    file.close();
}

/*
  Initialization of static members of GrayScottModelParameters
 */

bool GrayScottModelParameters::export_to_file = true;

double GrayScottModelParameters::Du = 0.16;
double GrayScottModelParameters::Dv = 0.08;
double GrayScottModelParameters::F = 0.035;
double GrayScottModelParameters::k = 0.06;

KeyframeStepper::DistributionMode GrayScottModelParameters::keyframeDistributionMode = KeyframeStepper::Linear;

uint GrayScottModelParameters::keyframes = 100;
uint GrayScottModelParameters::size = 128;
uint GrayScottModelParameters::timesteps = 10000;
