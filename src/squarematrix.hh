#ifndef __SQUAREMATRIX_HH__
#define __SQUAREMATRIX_HH__


#include <iostream>
using namespace std;


typedef unsigned int uint;


/*
  SquareMatrix represents a quadratic matrix. Element access is provided through
  the () operator and is implemented periodically, that is indices bigger that the 
  actual size of even negative ones are allowed (e.g. A(-1,0) == A(N-1,0) if N is
  the size of the matrix).
 */
class SquareMatrix {

public:

  /*
    construct SquareMatrix from given size and a default value for the elements
  */
  SquareMatrix(unsigned int size, double value=0.0) {
    m_size = size;
    m_data = new double*[m_size];
    for (uint i = 0; i < m_size; ++i) {
      m_data[i] = new double[m_size];
      for (uint j = 0; j < m_size; ++j) {
	m_data[i][j] = value;
      }
    }
  }

  /*
    construct SquareMatrix from given size and two dimensional data array
   */
  SquareMatrix(unsigned int size, double** data) {
    m_size = size;
    m_data = new double*[m_size];
    for (uint i = 0; i < m_size; ++i) {
      m_data[i] = new double[m_size];
      for (uint j = 0; j < m_size; ++j) {
	m_data[i][j] = data[i][j];
      }
    }
  }

  /*
    free memory
   */
  ~SquareMatrix() {
    if (m_data != 0) {
      for (uint i = 0; i < m_size; ++i) {
	delete[] m_data[i];
	m_data[i] = 0;
      }
      delete[] m_data;
      m_data = 0;
    }
  }

  /*
    return data array
   */
  inline double** data() { return m_data; }

  /*
    find maximum value
   */
  double max() const {
    double max = 0;

    for (uint i = 0; i < m_size; ++i) {
      for (uint j = 0; j < m_size; ++j) {
	if (m_data[i][j] > max) {
	  max = m_data[i][j];
	}
      }
    }

    return max;
  }

  /*
    element access
  */
  double& operator() (int i, int j) {    
    /*
    if (i < 0 || i >= m_size) {
      //i = rearrange_index(i);
      while (i < 0)
	i += m_size;
      while (i >= m_size)
	i -= m_size;
    }

    if (j < 0 || j >= m_size) {
      //j = rearrange_index(j);
      while (j < 0)
	j += m_size;
      while (j >= m_size)
	j -= m_size;
      }
    */

    if (i == -1)
      i += m_size;
    else if (i == m_size)
      i -= m_size;
	
    if (j == -1)
      j = m_size - 1;
    else if (j == m_size)
    j = 0;

    return m_data[i][j];
  }

  /*
    return matrix dimension
  */
  inline int size() const { return m_size; }

private:

  /*
    allow for periodic indexing
  */
  int rearrange_index(int i) {
    // handle negative indices
    while (i < 0) {
      i += m_size;
    }

    // handle indices greater than m_size-1
    while (i >= m_size) {
      i -= m_size;
    }

    return i;
  }

  double** m_data;
  uint m_size;
  
};


#endif // __SQUAREMATRIX_HH__
