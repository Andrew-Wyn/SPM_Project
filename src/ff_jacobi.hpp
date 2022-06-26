#include <vector>

using namespace std;

#ifndef FF_JACOBI_H
#define FF_JACOBI_H

void ff_jacobi_method(vector<vector<float>> &A, vector<float> &b, float eps, int max_iterations, bool distance_stop, bool debug, vector<float> &res, int nw);

#endif
