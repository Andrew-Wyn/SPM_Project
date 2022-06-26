#include <vector>

using namespace std;

#ifndef FF_JACOBI_H
#define FF_JACOBI_H

void ff_jacobi_method(vector<vector<double>> &A, vector<double> &b, double eps, int max_iterations, bool distance_stop, bool debug, vector<double> &res, int nw);

#endif
