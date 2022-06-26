#include <vector>

using namespace std;

#ifndef SEQ_JACOBI_H
#define SEQ_JACOBI_H

void seq_jacobi_method(vector<vector<double>> &A, vector<double> &b, double eps, int max_iterations, bool distance_stop, bool debug, vector<double> &res);

#endif
