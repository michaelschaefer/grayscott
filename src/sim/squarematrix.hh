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

    double max() const {
        double entry;
        double max = m_data[0][0];

        for (uint i = 0; i < m_size; ++i) {
            for (uint j = 0; j < m_size; ++j) {
                entry = m_data[i][j];
                if (entry > max) {
                    max = entry;
                }
            }
        }

        return max;
    }

    double min() const {
        double entry;
        double min = m_data[0][0];

        for (uint i = 0; i < m_size; ++i) {
            for (uint j = 0; j < m_size; ++j) {
                entry = m_data[i][j];
                if (entry < min) {
                    min = entry;
                }
            }
        }

        return min;
    }

    double& operator() (uint i, uint j) {
        if (i > m_size - 1) {
            i = m_size - 1;
        }

        if (j > m_size - 1) {
            j = m_size - 1;
        }

        return m_data[i][j];
    }

    void scale(double lower=0.0, double upper = 1.0) {
        double delta;
        double entry;
        double min = m_data[0][0];
        double max = min;

        for (uint i = 0; i < m_size; ++i) {
            for (uint j = 0; j < m_size; ++j) {
                entry = m_data[i][j];

                if (entry < min) {
                    min = entry;
                }

                if (entry > max) {
                    max = entry;
                }
            }
        }

        delta = (upper - lower) / (max - min);

        for (uint i = 0; i < m_size; ++i) {
            for (uint j = 0; j < m_size; ++j) {
                m_data[i][j] = (m_data[i][j] - min) * delta + lower;
            }
        }
    }


    inline double** data() { return m_data; }
    inline int size() const { return m_size; }

    void toUChar(uchar* u) {
        for (uint i = 0; i < m_size; ++i) {
            for (uint j = 0; j < m_size; ++j) {
                u[i * m_size + j] = uchar(m_data[i][j]);
            }
        }
    }


private:

    double** m_data;
    uint m_size;

};


#endif // __SQUAREMATRIX_HH__
