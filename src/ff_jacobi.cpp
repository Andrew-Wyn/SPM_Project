#include <iostream>
#include <cmath>
#include <vector>

#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>

#include "ff_jacobi.hpp"
#include "utils.hpp"

using namespace std;
using namespace ff;

void ff_jacobi_method(vector<vector<float>> &A, vector<float> &b, float eps, int max_iterations, bool distance_stop, bool debug, vector<float> &res, int nw) {
    int k = 0;

    int n = A.size();
    
    // initialization variables
    vector<float> x_k(n);
    vector<float> x_prev(n);

    // getting underling data, return array of float, faster to deal with
    auto A_data = A.data();
    auto b_data = b.data();
    auto x_prev_data = x_prev.data();
    auto x_k_data = x_k.data();

    // setting up the parallel for in FastFlow
    ParallelFor pf(nw);

    int chunk_size = n/nw;

    while ( true ) {
        // exit it the max iterations are reached
        if (k == max_iterations) {
            if (debug)
                cout << "WARNING: out of iterations !!" << endl;

            break;
        }

        // parallel for - map pattern
        pf.parallel_for(0, n, 1, chunk_size,
        [&](const long i){
            float sigma = 0;

            for (int j=0; j<n; j++) {
                if  ( j!=i )
                    sigma = sigma + A_data[i][j]*x_prev_data[j];
            }

            x_k_data[i] = (b_data[i] - sigma)/A_data[i][i];
        },
        nw);

        if (distance_stop) {
            // compute the euclidean distance between x_k and x_prev
            auto dist = euclidean_distance(x_k, x_prev);

            if (debug)
                cout << "Distance : " + to_string(dist) << endl;

            if (dist <= eps)
                break;
        }

        k++;

        // switch the pointers to data's vector to change the arrays pourpose
        auto tmp_data = x_prev_data;
        x_prev_data = x_k_data;
        x_k_data = tmp_data;
    }

    // copying out the result of the method
    res = x_k;
}