#include <iostream>
#include <cmath>
#include <vector>

#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>

using namespace std;
using namespace ff;

void ff_jacobi_method(vector<vector<double>> &A, vector<double> &b, double eps, vector<double> &res, int nw_map, int nw_reduce) {
    int k = 0;

    int n = A.size();

    vector<double> x_start = {0,0,0};
    vector<double> x_k(n);
    vector<double> x_prev(n);
    x_k = x_start;

    auto A_data = A.data();
    auto b_data = b.data();

    // setting up the parallel for in FastFlow
    ParallelFor pf(nw_map);

    while ( true ) {
        x_prev = x_k;

        auto x_prev_data = x_prev.data();
        auto x_k_data = x_k.data();

        // parallel for - map pattern
        pf.parallel_for(0, n, 1, 0,
        [&](const long i){
            double sigma = 0;

            for (int j=0; j<n; j++) {
                if  ( j!=i )
                    sigma = sigma + A_data[i][j]*x_prev_data[j];
            }

            /*
            // setting up the parallel for reduce in FastFlow
            ParallelForReduce<double> pfr(nw_reduce);

            // reduce pattern
            pfr.parallel_reduce(sigma, 0, 0, n, 1, 0, 
            [&](const long j, double &local_sigma){
                if  ( j!=i )
                    local_sigma = A_data[i][j]*x_prev_data[j];
            },
            [](double &s, const double partial) {
                s += partial;
            },
            nw_reduce);
            */

            x_k[i] = (b_data[i] - sigma)/A_data[i][i];
        },
        nw_map);

        auto dist = euclidean_distance(x_k, x_prev);

        cout << dist << endl;

        if (k == 100) {
            cout << "WARNING: out of iterations !!" << endl;
            break;
        }
        if (dist <= eps)
            break;
        k ++;
    }

    res = x_k;
}