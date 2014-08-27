#include <fstream>
#include <iostream>
#include <stdio.h>


using namespace std;


// forward declaration of create_matrix
double** create_matrix(double);
void evolve(double**, double**, double, double);
void export_data(double**);
void initial_values(double**, double**);
void laplacian(double**, double**);
void solve(double, double);


// global constants
const int N = 128;
const int timesteps = 10000;
const double Du = 0.16;
const double Dv = 0.08;


// global variables
double** Lu = create_matrix(0.0);
double** Lv = create_matrix(0.0);


/*
  create 2d array filled with a given value
*/
double** create_matrix(double value) {
  double** M = new double*[N];
  for (int i = 0; i < N; ++i) {
    M[i] = new double[N];
    for (int j = 0; j < N; ++j) {      
      M[i][j] = value;
    }
  }
  return M;
}


/*
  evolve the PDE system for one timestep
*/
void evolve(double** u, double** v, double F, double k) {  
  double u_entry = 0.0;
  double v_entry = 0.0;
  double uvv = 0.0;

  laplacian(u, Lu);
  laplacian(v, Lv);

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {      
      u_entry = u[i][j];
      v_entry = v[i][j];
      uvv = u_entry * v_entry * v_entry;
      u[i][j] = u_entry + Du * Lu[i][j] - uvv + F * (1 - u_entry);
      v[i][j] = v_entry + Dv * Lv[i][j] + uvv - (F + k) * v_entry;
    }
  }
}


int max_color(double** u) {
  double max = 0.0;
  int ret = 0;

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; ++j) {
      if (255 * u[i][j] > max) {
	max = 255 * u[i][j];
      }
    }
  }

  ret = int(max);
  if (ret == 0) {
    ret = 1;
  }
  return ret;
}


/*
  export data to disc
 */
void export_data(double** u, string filename) {
  ofstream file;
  file.open(filename.c_str());
  
  // write header
  file << "P2" << endl;
  file << "# output from grayscott" << endl;
  file << N << " " << N << endl;
  file << max_color(u) << endl;
  
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {      
      file << int(255 * u[i][j]) << " ";
    }
    file << endl;
  }
  file.close();
}


/*
  fill system variables u and v with initial values
 */
void initial_values(double** u, double** v) {
  int r = 20;
  double start = N / 2 - r;
  double end = N / 2 + r;

  for (int i = start; i < end; ++i) {
    for (int j = start; j < end; ++j) {
      u[i][j] = 0.5; // plus random noise
      v[i][j] = 0.25; // plus random noise
    }
  }
}


/*
  calculate the Laplacian of u and store it in Lu
 */
void laplacian(double** w, double** L) {
  // corners
  L[0][0] = w[0][N-1] + w[0][1] + w[N-1][0] + w[1][0] - 4.0 * w[0][0];
  L[0][N-1] = w[0][N-2] + w[0][0] + w[N-1][N-1] + w[1][N-1] - 4.0 * w[0][N-1];
  L[N-1][0] = w[N-1][N-1] + w[N-1][1] + w[N-2][0] + w[0][0] - 4.0 * w[N-1][0];
  L[N-1][N-1] = w[N-1][N-2] + w[N-1][0] + w[N-2][N-1] + w[0][N-1] - 4.0 * w[N-1][N-1];

  for (int i = 1; i < N-1; ++i) {
    // inner boundary
    L[0][i] = w[0][i-1] + w[0][i+1] + w[N-1][i] + w[1][i] - 4.0 * w[0][i];
    L[N-1][i] = w[N-1][i-1] + w[N-1][i+1] + w[N-2][i] + w[0][i] - 4.0 * w[N-1][i];
    L[i][0] = w[i][N-1] + w[i][1] + w[i-1][0] + w[i+1][0] - 4.0 * w[i][0];
    L[i][N-1] = w[i][N-2] + w[i][0] + w[i-1][N-1] + w[i+1][N-1] - 4.0 * w[i][N-1];

    for (int j = 1; j < N-1; ++j) {            
      // interior
      L[i][j] = w[i-1][j] + w[i+1][j] + w[i][j-1] + w[i][j+1] - 4.0 * w[i][j];
    }
  }
}


void solve(double F, double k) {
  double** u = create_matrix(1.0);  
  double** v = create_matrix(0.0);
  initial_values(u, v);
  
  for (int step = 0; step < timesteps; ++step) {
    evolve(u, v, F, k);
  }

  char buffer[50];
  sprintf(buffer, "./study/F%03d-k%03d.pgm", int(1000*F), int(1000*k));
  export_data(v, buffer);

  printf("Parameter F=%03d, k=%03d finished\n", int(1000*F), int(1000*k));
}


/*
  do the evolution
 */
int main() {
  for (double F = 5; F <= 30; F += 5) {
    for (double k = 45; k <= 65; k += 5) {
      solve(F / 1000, k / 1000);
    }
  }
  return 0;
}
