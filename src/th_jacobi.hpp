#include <vector>

using namespace std;

#ifndef TH_JACOBI_H
#define TH_JACOBI_H

void th_jacobi_method(vector<vector<double>> &A, vector<double> &b, double eps, int max_iterations, bool distance_stop, bool debug, vector<double> &res, int nw);

#endif
