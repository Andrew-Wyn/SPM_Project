#include <iostream>
#include <cmath>
#include "utimer.cpp"

using namespace std;

template <size_t n>
double euclidean_distance(double x[n], double y[n]) {
    double dist = 0;

    for (int i=0; i<n; i++) {
        dist += pow((x[i] - y[i]), 2);
    }

    return sqrt(dist);
}

template <size_t n>
void print_vector(double x[n]) {
    for (int i=0; i<n; i++) {
        cout << x[i] << " ";
    }

    cout << endl;
}

template <size_t n>
void copy_array(double x[n], double y[n]) {
    for (int i=0; i<n; i++) {
        x[i] = y[i];
    }
}

template <size_t n>
void jacobi_method(double x_start[n], double A[n][n], double b[n], double eps) {
    int k = 0;

    double x_k[n];
    double x_prev[n];
    copy_array<n>(x_k, x_start);

    while ( true ) {
        copy_array<n>(x_prev, x_k);
        for (int i=0; i<n; i++) {
            int sigma = 0;
            for (int j=0; j<n; j++) {
                if  ( j==i )
                    continue;
                sigma = sigma + A[i][j]*x_prev[j];
            }
            x_k[i] = (b[i] - sigma)/A[i][i];
        }
        auto dist = euclidean_distance<n>(x_k, x_prev);
        print_vector<n>(x_prev);
        print_vector<n>(x_k);
        cout << dist << endl;
        if (k == 100)
        break;
        if (dist == 0.0)
            break;
        k ++;
    }
}

int main() {
    cout << "SEQUENTIAL JACOBI METHOD" << endl;
    const int n = 3;
    double A[n][n] = {{0.5, 0, 0}, {0, 0.5, 0}, {0 , 0, 0.5}};
    double x_start[n] = {0,0,0};
    double b[n] = {1, 1, 1};
    {
        auto timer = utimer("jacobi with n = 4");

        jacobi_method<n>(x_start, A, b, 0.000000001);
    }
}
