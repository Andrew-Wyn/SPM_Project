#include <iostream>
#include <cmath>
#include "utimer.cpp"

using namespace std;

template <size_t n>
float euclidean_distance(float x[n], float y[n]) {
    float dist = 0;

    for (int i=0; i<n; i++) {
        cout << x[i] << endl;
        dist += pow((x[i] - y[i]), 2);
    }

    return sqrt(dist);
}

template <size_t n>
void jacobi_method(float x_start[n], float A[n][n], float b[n], float eps) {
    int k = 0;

    auto x_k = x_start;

    while ( true ) {
        auto x_prev = x_k;
        float x_k[n];
        for (int i=0; i<n; i++) {
            int sigma = 0;
            for (int j=0; j<n; j++) {
                if  ( j==i )
                    continue;
                sigma = sigma + A[i][j]*x_prev[j];
            }
            x_k[i] = (b[i] - sigma)/A[i][i];
        }
        if (euclidean_distance<n>(x_k, x_prev) <= eps)
            break;
    }
}

int main() {
    cout << "SEQUENTIAL JACOBI METHOD" << endl;
    const int n = 4;
    float A[n][n] = {{2,1,2,3}, {4,5,6,7}, {8,9,10,11}, {1,2,3,4}};
    float x_start[n] = {0,0,0,0};
    float b[n] = {3, 4, 3, 3};
    {
        auto timer = utimer("jacobi with n = 4");

        jacobi_method<n>(x_start, A, b, 1.0f);
    }
}
