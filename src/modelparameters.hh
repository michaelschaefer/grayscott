#ifndef __MODELPARAMETERS_HH__
#define __MODELPARAMETERS_HH__


#include <iostream>


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

  inline static double Du() { return m_Du; }
  inline static double Dv() { return m_Dv; }
  inline static double F() { return m_F; }
  inline static double k() { return m_k; }
  inline static uint N() { return m_N; }

  static bool set_Du(double Du) {
    if (Du <= 0) {
      cerr << "Diffusion coefficient Du must be positive" << endl;
      return false;
    }    
    
    m_Du = Du;
    return true;
  }

  static bool set_Dv(double Dv) {
    if (Dv <= 0) {
      cerr << "Diffusion coefficient Dv must be positive" << endl;
      return false;
    }    
    
    m_Dv = Dv;
    return true;
  }

  static bool set_F(double F) {
    if (F < 0) {
      cerr << "Feed rate F must be non-negative" << endl;
      return false;
    }    
    
    m_F = F;
    return true;
  }

  static bool set_k(double k) {
    if (k < 0) {
      cerr << "Rate constant k must be non-negative" << endl;
      return false;
    }    
    
    m_k = k;
    return true;
  }

  static bool set_N(uint N) {
    if (N == 0) {
      cerr << "Number of grid elements N must be positive" << endl;
      return false;
    }    
    
    m_N = N;
    return true;
  }  

private:

  static double m_Du;
  static double m_Dv;
  static double m_F;
  static double m_k;
  static uint m_N;

};


/*
  Initialization of static members of GrayScottModelParameters
 */
double GrayScottModelParameters::m_Du = 0.16;
double GrayScottModelParameters::m_Dv = 0.08;
double GrayScottModelParameters::m_F = 0.035;
double GrayScottModelParameters::m_k = 0.06;
uint GrayScottModelParameters::m_N = 128;


#endif // __MODEL_HH__
