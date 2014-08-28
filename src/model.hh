#ifndef __MODEL_HH__
#define __MODEL_HH__


#include <chrono>
#include <random>
#include "modelparameters.hh"
#include "squarematrix.hh"


typedef unsigned int uint;


/*
  Numerical implementation of the Gray-Scott model for pattern formation. The
  model consists of two couple partial differential equations for chemical
  species u and v, namely
        
      ∂u/∂t = Du·∇²u - u·v² + F·(1 - u),
      ∂v/∂t = Dv·∇²v + u·v² - (F + k)·v,      
  
  and is equipped with periodic boundary conditions and specially chosen 
  initial values.

  The numerical approximations of u and v are calculated using finite 
  differences and the usual 5-point stencil for the Laplacian as well as simple
  formward Euler for the time derivatives.
 */
class GrayScottModel {

public:

  GrayScottModel() {
    m_N = GrayScottModelParameters::N();
    
    m_Lu = new double*[m_N];
    m_Lv = new double*[m_N];
    m_u = new double*[m_N];
    m_v = new double*[m_N];
    for (uint i = 0; i < m_N; ++i) {
      m_Lu[i] = new double[m_N];
      m_Lv[i] = new double[m_N];
      m_u[i] = new double[m_N];
      m_v[i] = new double[m_N];
      for (uint j = 0; j < m_N; ++j) {
	m_Lu[i][j] = 0.0;
	m_Lv[i][j] = 0.0;
	m_u[i][j] = 1.0;
	m_v[i][j] = 0.0;
      }
    }

    set_initial_values();
  }

  ~GrayScottModel() {
    free_memory(m_u);
    free_memory(m_v);
    free_memory(m_Lu);
    free_memory(m_Lv);
  }

  void evolve() {
    double Lu = 0.0;
    double Lv = 0.0;
    double u = 0.0;
    double uvv = 0.0;
    double v = 0.0;

    laplacian(m_u, m_Lu);
    laplacian(m_v, m_Lv);

    for (uint i = 0; i < m_N; ++i) {
      for (uint j = 0; j < m_N; ++j) {
	Lu = m_Lu[i][j];
	Lv = m_Lv[i][j];
	u = m_u[i][j];
	v = m_v[i][j];
	uvv = u * v * v;	
	
	m_u[i][j] += GrayScottModelParameters::Du() * Lu - uvv + 
	  GrayScottModelParameters::F() * (1 - u);
	m_v[i][j] += GrayScottModelParameters::Dv() * Lv + uvv -
	  (GrayScottModelParameters::F() + GrayScottModelParameters::k()) * v;
      }
    }
  }

  void reset() {
    for (uint i = 0; i < m_N; ++i) {
      for (uint j = 0; j < m_N; ++j) {
	m_Lu[i][j] = 0.0;
	m_Lv[i][j] = 0.0;
	m_u[i][j] = 1.0;
	m_v[i][j] = 0.0;
      }
    }
    set_initial_values();
  }

  SquareMatrix u() { return SquareMatrix(m_N, m_u); }
  SquareMatrix v() { return SquareMatrix(m_N, m_v); }

private: 

  void free_memory(double** u) {
    if (u != 0) {
      for (uint i = 0; i < m_N; ++i) {
	if (u[i] != 0) {
	  delete[] u[i];
	  u[i] = 0;
	}
      }
      delete[] u;
      u = 0;
    }
  }

  void laplacian(double** w, double** L) {
    // corners
    L[0][0] = w[0][m_N-1] + w[0][1] + w[m_N-1][0] + w[1][0] - 4.0 * w[0][0];
    L[0][m_N-1] = w[0][m_N-2] + w[0][0] + w[m_N-1][m_N-1] + w[1][m_N-1] - 4.0 * w[0][m_N-1];
    L[m_N-1][0] = w[m_N-1][m_N-1] + w[m_N-1][1] + w[m_N-2][0] + w[0][0] - 4.0 * w[m_N-1][0];
    L[m_N-1][m_N-1] = w[m_N-1][m_N-2] + w[m_N-1][0] + w[m_N-2][m_N-1] + w[0][m_N-1] - 4.0 * w[m_N-1][m_N-1];

    for (int i = 1; i < m_N-1; ++i) {
      // inner boundary
      L[0][i] = w[0][i-1] + w[0][i+1] + w[m_N-1][i] + w[1][i] - 4.0 * w[0][i];
      L[m_N-1][i] = w[m_N-1][i-1] + w[m_N-1][i+1] + w[m_N-2][i] + w[0][i] - 4.0 * w[m_N-1][i];
      L[i][0] = w[i][m_N-1] + w[i][1] + w[i-1][0] + w[i+1][0] - 4.0 * w[i][0];
      L[i][m_N-1] = w[i][m_N-2] + w[i][0] + w[i-1][m_N-1] + w[i+1][m_N-1] - 4.0 * w[i][m_N-1];

      for (int j = 1; j < m_N-1; ++j) {            
	// interior
	L[i][j] = w[i-1][j] + w[i+1][j] + w[i][j-1] + w[i][j+1] - 4.0 * w[i][j];
      }
    }
  }

void set_initial_values(uint r=10, double noise=0.01) {
    int a = m_N / 2 - r;
    int b = m_N / 2 + r;    
    long seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_real_distribution<double> distribution(0.0, 2*noise);
    
    for (uint i = a; i < b; ++i) {
      for (uint j = a; j < b; ++j) {
	m_u[i][j] = 0.5 + distribution(generator) - noise;
	m_v[i][j] = 0.25 + distribution(generator) - noise;
      }
    }
  }

  double** m_Lu;
  double** m_Lv;
  double** m_u;
  double** m_v;

  uint m_N;  

};


#endif // __MODEL_HH__
