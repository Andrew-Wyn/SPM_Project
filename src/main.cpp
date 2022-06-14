#include <iostream>
#include <cmath>
#include <random>
#include <vector>

#include "utimer.cpp"
#include "utils.cpp"
#include "seq_jacobi.cpp"
#include "ff_jacobi.cpp"
#include "th_jacobi.cpp"

using namespace std;

int main() {
    //srand(time(NULL));
    srand(123);
    cout << "SEQUENTIAL JACOBI METHOD" << endl;
    const int n = 1000;

    // instantiate matrix A
    vector<vector<double>> A(n);
    for (int i=0; i<n; i++) {
        A[i] = vector<double>(n);
    }

    // instantiate vector b
    vector<double> b(n);

    // instantiate vector res
    vector<double> res_seq(n);
    vector<double> res_ff(n);
    vector<double> res_th(n);

    initialize_problem(A, b, -10, 10);

    print_matrix(A);
    print_vector(b);

    cout << int(is_diagonally_dominant(A)) << endl;

    {
        auto timer_seq = utimer("SEQ: jacobi with n = 1000");

        seq_jacobi_method(A, b, 1.0e-6, res_seq);
    }
    
    print_vector(res_seq);

    {
        auto timer_ff = utimer("FF: jacobi with n = 1000");

        ff_jacobi_method(A, b, 1.0e-6, res_ff, 32, 10);
    }
    
    print_vector(res_ff);

    {
        auto timer_th = utimer("TH: jacobi with n = 1000");

        th_jacobi_method(A, b, 1.0e-6, res_th, 32, 10);
    }
    
    print_vector(res_th);

}