#include <iostream>
#include <cmath>
#include <random>
#include <vector>
#include <string>
#include <chrono>

#include "utimer.hpp"
#include "utils.hpp"
#include "seq_jacobi.hpp"
#include "ff_jacobi.hpp"
#include "th_jacobi.hpp"
#include "th2_jacobi.hpp"

using namespace std;

int main(int argc, char** argv) {

    bool debug;
    
    if(char* env_debug_c = std::getenv("DEBUG")) {
        string env_debug(env_debug_c);

        if (! env_debug.compare("1")) // env_debug == "1"
            debug = true;
        else
            debug = false;
    }
    else {
        debug = false;
    }

    // Getting Args parameters
    int n = atoi(argv[1]);
    int nw = atoi(argv[2]);
    int max_iterations = atoi(argv[3]);
    int distance_stop = atoi(argv[4]);

    //srand(time(NULL));
    srand(123);

    // instantiate matrix A
    vector<vector<float>> A(n);
    for (int i=0; i<n; i++) {
        A[i] = vector<float>(n);
    }

    // instantiate vector b
    vector<float> b(n);

    // instantiate vector res
    vector<float> res_seq(n);
    vector<float> res_ff(n);
    vector<float> res_th(n);
    vector<float> res_th2(n);

    // initialize the problem
    initialize_problem(A, b, -10, 10);
    
    if (debug) {
        cout << "Matrix A:" << endl;
        print_matrix(A);
        if (int(is_diagonally_dominant(A)) == 1) {
            cout << "Matrix A is diagonally dominant"  << endl;
        }
        cout << "Vector b:" << endl;
        print_vector(b);

        cout << endl;
    }

    // sequential implementation run
    if (nw == 1) {
        if (debug)
            cout << "SEQ: starting..." << endl;

        auto timer_seq = utimer("SEQ: jacobi with n = " + to_string(n) + ", nw = " + to_string(nw));

        seq_jacobi_method(A, b, 1.0e-6, max_iterations, distance_stop, debug, res_seq);
    }
    
    if (nw == 1 && debug) {
        cout << "SEQ: Result" << endl;
        print_vector(res_seq);
        cout << endl;
    }

    // fast flow implementation run
    {
        if (debug)
            cout << "FF: starting..." << endl;

        auto timer_ff = utimer("FF: jacobi with n = " + to_string(n) + ", nw = " + to_string(nw));

        ff_jacobi_method(A, b, 1.0e-6, max_iterations, distance_stop, debug, res_ff, nw);
    }
    
    if (debug) {
        cout << "FF: Result" << endl;
        print_vector(res_ff);
        cout << endl;
    }

    // Thread (pool) implementation run
    {
        if (debug)
            cout << "TH: starting..." << endl;

        auto timer_th = utimer("TH: jacobi with n = " + to_string(n) + ", nw = " + to_string(nw));

        th_jacobi_method(A, b, 1.0e-6, max_iterations, distance_stop, debug, res_th, nw);
    }
    
    if (debug) {
        cout << "TH: Result" << endl;
        print_vector(res_th);
        cout << endl;
    }

    // Thread (barrier) implementation run
    {
        if (debug)
            cout << "TH2: starting..." << endl;

        auto timer_th2 = utimer("TH2: jacobi with n = " + to_string(n) + ", nw = " + to_string(nw));

        th2_jacobi_method(A, b, 1.0e-6, max_iterations, distance_stop, debug, res_th2, nw);
    }

    if (debug) {
        cout << "TH2: Result" << endl;
        print_vector(res_th2);
        cout << endl;
    }
}