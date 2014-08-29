#ifndef __COLORMAP_HH__
#define __COLORMAP_HH__


typedef unsigned int uint;


/*
  Color represents colors in the RGB format with 8bit per color.
 */
class Color {

public:

  Color(uint red=0, uint green=0, uint blue=0) {
    m_blue = blue;
    m_green = green;
    m_red = red;    
  }

  inline uint blue() const { return m_blue; }
  inline uint green() const { return m_green; }
  inline uint red() const { return m_red; }

  uint& operator[] (uint index) {
    if (index == 0) {
      return m_red;
    } else if (index == 1) {
      return m_green;
    } else {
      return m_blue;
    }    
  }

private:
  
  uint m_blue;
  uint m_green;
  uint m_red;  

};


/*
  Base class for linear color shading with an intermediate color, that is 
  an intensity value x in [0,0.5] gives a linear interpolation between a
  start and an intermediate color while for x in (0.5,] the interpolation is
  done from an intermediate to an end color.
 */
class Colormap {
  
public:

  Colormap(Color start=Color(0,0,0), Color mid=Color(127,127,127), 
	   Color end=Color(255,255,255), double min=0.0, double max=1.0) {
    m_end = end;
    m_mid = mid;
    m_start = start;

    for (uint i = 0; i < 3; ++i) {
      m_dmid[i] = m_mid[i] - m_start[i];
      m_dend[i] = end[i] - m_mid[i];
    }

    m_delta = max - min;
    m_max = max;
    m_min = min;    
  }

  Color map(double intensity) {
    Color color;
    double x = (intensity - m_min) / m_delta;

    if (x < 0.0) {
      return m_start;
    } else if (x > 1.0) {
      return m_end;
    }
    
    if (x < 0.5) {
      for (uint i = 0; i < 3; ++i) {
	color[i] = m_start[i] + 2 * x * m_dmid[i];
      }
    } else {
      for (uint i = 0; i < 3; ++i) {
	color[i] = m_mid[i] + 2 * (x - 0.5) * m_dend[i];
      }
    }
    
    return color;
  }

  inline double max() const { return m_max; }
  inline double min() const { return m_min; }

  inline void set_max(double max) { m_max = max; m_delta = m_max - m_min; }
  inline void set_min(double min) { m_min = min; m_delta = m_max - m_min; }

private:

  Color m_end;
  Color m_mid;
  Color m_start;

  double m_delta;
  double m_max;
  double m_min;  

  int m_dmid[3];
  int m_dend[3];

};


/*
  ColormapBlueRed is a Colormap fading from a dark blue over a very light gray
  (quasi white) to a dark red.
 */
class ColormapBlueRed : public Colormap {

public:    
  
  ColormapBlueRed(double min=0.0, double max=1.0) 
    : Colormap(Color(5,48,97), Color(246,246,246), Color(186,61,62), min, max) {}

};


#endif // __COLORMAP_HH__
