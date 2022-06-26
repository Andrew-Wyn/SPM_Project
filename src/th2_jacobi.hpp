#include <vector>

using namespace std;

#ifndef TH2_JACOBI_H
#define TH2_JACOBI_H

void th2_jacobi_method(vector<vector<double>> &A, vector<double> &b, double eps, int max_iterations, bool distance_stop, bool debug, vector<double> &res, int nw);

#endif