#include <stdio.h>
#include "exporter.hh"
#include "model.hh"
#include "modelparameters.hh"


typedef unsigned int uint;


uint T = 10000;


int main() {    
  GrayScottModelParameters::set_N(128);

  for (uint F = 0; F <= 100; F += 10) {
    for (uint k = 0; k <= 100; k += 10) {
      GrayScottModelParameters::set_F(F / 1000.0);
      GrayScottModelParameters::set_k(k / 1000.0);
      
      GrayScottModel model;

      for (uint step = 0; step < T; ++step) {
	model.evolve();
      }

      char buffer[50];
      sprintf(buffer, "./study/F%03d-k%03d.pgm", F, k);  
      Exporter::write_image(model.v(), buffer); 
      printf("Parameter F=%03d, k=%03d finished\n", F, k);
    }
  }    

  return 0;
}
