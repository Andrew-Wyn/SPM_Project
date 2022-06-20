#include <iostream>
#include <cmath>
#include <vector>

#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>

using namespace std;
using namespace ff;

void ff_jacobi_method(vector<vector<double>> &A, vector<double> &b, double eps, int max_iterations, bool distance_stop, bool debug, vector<double> &res, int nw) {
    int k = 0;

    int n = A.size();

    vector<double> x_k(n);
    vector<double> x_prev(n);

    auto A_data = A.data();
    auto b_data = b.data();

    auto x_prev_data = x_prev.data();
    auto x_k_data = x_k.data();

    // setting up the parallel for in FastFlow
    ParallelFor pf(nw);

    int chunk_size = n/nw;

    while ( true ) {
        // parallel for - map pattern
        pf.parallel_for(0, n, 1, chunk_size,
        [&](const long i){
            double sigma = 0;

            for (int j=0; j<n; j++) {
                if  ( j!=i )
                    sigma = sigma + A_data[i][j]*x_prev_data[j];
            }

            x_k_data[i] = (b_data[i] - sigma)/A_data[i][i];
        },
        nw);

        if (distance_stop) {
            auto dist = euclidean_distance(x_k, x_prev);

            if (debug)
                cout << "Distance : " + to_string(dist) << endl;

            if (dist <= eps)
                break;
        }

        if (k == max_iterations) {
            if (debug)
                cout << "WARNING: out of iterations !!" << endl;

            break;
        }

        k++;

        // swap the array
        auto tmp_data = x_prev_data;
        x_prev_data = x_k_data;
        x_k_data = tmp_data;
    }

    res = x_k;
}