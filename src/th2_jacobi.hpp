#include <vector>

using namespace std;

#ifndef TH2_JACOBI_H
#define TH2_JACOBI_H

void th2_jacobi_method(vector<vector<float>> &A, vector<float> &b, float eps, int max_iterations, bool distance_stop, bool debug, vector<float> &res, int nw);

#endif