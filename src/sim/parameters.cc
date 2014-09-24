#include <fstream>
#include <iostream>
#include <string>
#include "src/sim/parameters.hh"


void Parameters::load(string filename) {
    ifstream file(filename, ios_base::in | ios_base::binary);
    string path;
    uint mode;

    file >> Parameters::Model::Du;
    file >> Parameters::Model::Dv;
    file >> Parameters::Model::F;
    file >> Parameters::Model::k;    
    file >> Parameters::Model::size;
    file >> Parameters::Model::speedup;
    file >> Parameters::Export::exportToFile;
    file >> Parameters::Export::timesteps;
    file >> Parameters::Export::frames;
    file >> mode;
    file >> path;
    file.close();

    Parameters::Export::frameDistributionMode = FrameDistribution::modeFromInt(mode);
    Parameters::Export::path = QString(path.c_str());
}

void Parameters::save(string filename) {
    ofstream file(filename, ios_base::out | ios_base::binary);
    file << Parameters::Model::Du << endl;
    file << Parameters::Model::Dv << endl;
    file << Parameters::Model::F << endl;
    file << Parameters::Model::k << endl;    
    file << Parameters::Model::size << endl;
    file << Parameters::Model::speedup << endl;
    file << Parameters::Export::exportToFile << endl;
    file << Parameters::Export::timesteps << endl;
    file << Parameters::Export::frames << endl;
    file << Parameters::Export::frameDistributionMode << endl;
    file << Parameters::Export::path.toStdString() << endl;
    file.close();
}


/*
  Initialization of static members
 */

bool Parameters::Export::exportToFile = false;

double Parameters::Model::Du = 0.16;
double Parameters::Model::Dv = 0.08;
double Parameters::Model::F = 0.035;
double Parameters::Model::k = 0.06;

FrameDistribution::Mode Parameters::Export::frameDistributionMode =
        FrameDistribution::Linear;

QString Parameters::Export::path = "";

uint Parameters::Export::frames = 100;
uint Parameters::Model::size = 128;
uint Parameters::Model::speedup = 10;
uint Parameters::Export::timesteps = 10000;
