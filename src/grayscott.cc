#include <stdio.h>
#include <stdlib.h>
#include "exporter.hh"
#include "model.hh"
#include "modelparameters.hh"


typedef unsigned int uint;


uint T = 10000;


void parse_arguments(int argc, char** argv) {
  if (argc < 1) {
    return;
  }

  switch (argc) {    
  default:
  case 5:
    T = atoi(argv[4]);
  case 4:
    GrayScottModelParameters::N = atoi(argv[3]);
  case 3:
    GrayScottModelParameters::k = atof(argv[2]);
  case 2:
    GrayScottModelParameters::F = atof(argv[1]);
    break;
  case 1:
    break;
  }
}


int main(int argc, char** argv) {
  parse_arguments(argc, argv);
      
  GrayScottModel model;
  
  for (uint step = 0; step < T; ++step) {
    model.evolve();
  }
  
  
  char buffer[50];
  uint F = int(1000 * GrayScottModelParameters::F);
  uint k = int(1000 * GrayScottModelParameters::k);

  if (GrayScottModelParameters::export_to_file == true) {
    sprintf(buffer, "./study/F%03d-k%03d.pgm", F, k);  
    Exporter::write_image(model.v(), buffer); 
  }

  return 0;
}
