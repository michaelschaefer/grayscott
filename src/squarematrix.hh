#ifndef __SQUAREMATRIX_HH__
#define __SQUAREMATRIX_HH__


#include <iostream>
using namespace std;


typedef unsigned int uint;


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
  double** data() { return m_data; }

  /*
    find maximum value
   */
  double max() const {
    double max = 0;

    for (uint i = 0; i < m_size; ++i) {
      for (uint j = 0; j < m_size; ++j) {
	if (m_data[i][j] < max) {
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
    if (i < 0 || i >= m_size) {
      i = rearrange_index(i);
    }

    if (j < 0 || j >= m_size) {
      j = rearrange_index(j);
    }

    return m_data[i][j];
  }

  /*
    return matrix dimension
  */
  int size() const { return m_size; }

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
