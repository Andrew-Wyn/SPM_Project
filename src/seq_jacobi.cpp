#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

void seq_jacobi_method(vector<vector<double>> &A, vector<double> &b, double eps, vector<double> &res) {
    int k = 0;

    int n = A.size();

    vector<double> x_start(n);
    vector<double> x_k(n);
    vector<double> x_prev(n);
    x_k = x_start;

    auto A_data = A.data();
    auto b_data = b.data();

    while ( true ) {
        x_prev = x_k;

        auto x_prev_data = x_prev.data();
        auto x_k_data = x_k.data();

        for (int i=0; i<n; i++) {
            double sigma = 0;
            for (int j=0; j<n; j++) {
                if  ( j!=i )
                    sigma = sigma + A_data[i][j]*x_prev_data[j];
            }
            x_k[i] = (b_data[i] - sigma)/A_data[i][i];
        }

        if (k == 100) {
            cout << "WARNING: out of iterations !!" << endl;
            break;
        }

        auto dist = euclidean_distance(x_k, x_prev);

        cout << dist << endl;

        if (dist <= eps)
            break;
        k ++;
    }

    res = x_k;
}