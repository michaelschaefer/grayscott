#ifndef __EXPORTER_HH__
#define __EXPORTER_HH__


#include <fstream>
#include "squarematrix.hh"


typedef unsigned int uint;


class Exporter {

public:

  static void write_image(SquareMatrix& data, string filename) {
    ofstream file;
    uint size = data.size();
    uint color[3];
    
    file.open(filename.c_str());
    
    // write pgm header
    file << "P3" << endl;
    file << "# image taken from grayscott" << endl;
    file << "# (c) 2014 Michael Schaefer (mail@michael-schaefer.org)" << endl;
    file << size << " " << size << endl;
    file << "255" << endl;

    for (uint i = 0; i < size; ++i) {
      for (uint j = 0; j < size; ++j) {	
	colormap(data(i,j), color);
	for (uint k = 0; k < 3; ++k) {
	  file << color[k] << " ";
	}
      }
      file << endl;
    }

    file.close();
  }

private:  

  static void colormap(double intensity, uint* color) {
    for (uint i = 0; i < 3; ++i) {
      color[i] = int(255 * intensity);
    }
  }

};


#endif // __EXPORTER_HH__
