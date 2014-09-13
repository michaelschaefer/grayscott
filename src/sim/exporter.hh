#ifndef __EXPORTER_HH__
#define __EXPORTER_HH__


#include <fstream>
#include "colormap.hh"
#include "squarematrix.hh"


typedef unsigned int uint;


/*
  Exporter serves as framework for writing images to disc. A colormap can be
  specified as an instance of Colormap or an object of a derived class of it.
  A rescale flag allows to select whether the upper limit of the colormap is 
  rescaled to the maximum of the actual data.
 */
class Exporter {

public:

  inline static void set_colormap(Colormap colormap) { m_colormap = colormap; }
  inline static void set_rescale_colormap(bool rescale) { m_rescale = rescale; }

  static void write_image(SquareMatrix data, string filename) {
    Color color;
    ofstream file;
    uint size = data.size();        

    if (m_rescale == true) {
      m_colormap.set_max(data.max());
    }
    
    // write pgm header
    file.open(filename.c_str());
    file << "P3" << endl;
    file << "# image taken from grayscott" << endl;
    file << "# (c) 2014 Michael Schaefer (mail@michael-schaefer.org)" << endl;
    file << size << " " << size << endl;
    file << "255" << endl;

    for (uint i = 0; i < size; ++i) {
      for (uint j = 0; j < size; ++j) {	
	color = m_colormap.map(data(i,j));
	for (uint k = 0; k < 3; ++k) {
	  file << color[k] << " ";
	}
      }
      file << endl;
    }

    file.close();
  }

private:  

  static bool m_rescale;
  static Colormap m_colormap;

};


/*
  Initialization of static members of Exporter
 */
bool Exporter::m_rescale = false;
Colormap Exporter::m_colormap = ColormapBlueRed();


#endif // __EXPORTER_HH__
