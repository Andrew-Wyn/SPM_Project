#include <vector>

using namespace std;

#ifndef SEQ_JACOBI_H
#define SEQ_JACOBI_H

void seq_jacobi_method(vector<vector<float>> &A, vector<float> &b, float eps, int max_iterations, bool distance_stop, bool debug, vector<float> &res);

#endif
