#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

void seq_jacobi_method(vector<vector<double>> &A, vector<double> &b, double eps, int max_iterations, bool distance_stop, bool debug, vector<double> &res) {
    int k = 0;

    int n = A.size();

    // initialization variables
    vector<double> x_k(n, 0);
    vector<double> x_prev(n, 0);

    // getting underling data, return array of double, faster to deal with
    auto A_data = A.data();
    auto b_data = b.data();
    auto x_prev_data = x_prev.data();
    auto x_k_data = x_k.data();

    while ( true ) {

        // Jacobi Iteration
        for (int i=0; i<n; i++) {
            double sigma = 0;
            for (int j=0; j<n; j++) {
                if  ( j!=i )
                    sigma = sigma + A_data[i][j]*x_prev_data[j];
            }
            x_k_data[i] = (b_data[i] - sigma)/A_data[i][i];
        }

        if (distance_stop) {
            // compute the euclidean distance between x_k and x_prev
            auto dist = euclidean_distance(x_k, x_prev);

            if (debug)
                cout << "Distance : " + to_string(dist) << endl;

            if (dist <= eps)
               break;
        }

        // exit it the max iterations are reached
        if (k == max_iterations) {
            if (debug)
                cout << "WARNING: out of iterations !!" << endl;
            
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