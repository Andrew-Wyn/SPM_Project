#include <iostream>
#include <cmath>
#include "utimer.cpp"
#include <random>
using namespace std;


template <size_t n>
double euclidean_distance(double x[n], double y[n]) {
    double dist = 0;

    for (int i=0; i<n; i++) {
        dist += pow((x[i] - y[i]), 2);
    }

    return sqrt(dist);
}

double random_in_range(int minimum, int maximum) {
    auto randValue = rand() / double(RAND_MAX); // random number in [0, 1]
    randValue = randValue*(maximum - minimum) + minimum;

    return randValue;
}

template <size_t n>
bool is_diagonally_dominant(double A[n][n]) {
    for (int i=0; i<n; i++) {
        double sum = 0;
        for (int j=0; j<n; j++) {
            if (j!=i)
                sum += abs(A[i][j]);
        }
        if (abs(A[i][i]) <= sum) {
            return false;
        }
    }


    return true;
}

template <size_t n>
void initialize_matrix_A(double A[n][n], double b[n], int minimum, int maximum) {

    // initialize matrix with random values
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            A[i][j] = random_in_range(minimum, maximum);
        }
        b[i] = random_in_range(minimum, maximum);
    }

    // make the matrix A strictly diagonally
    for (int i=0; i<n; i++) {

        // To store the sum of the current row
        double sum = 0;
        for (int j = 0; j < n; j++)
            if (j != i)
                sum += abs(A[i][j]);

        // Checking if the diagonal element is less
        // than the sum of non-diagonal element
        // then add their difference to the result
        if (abs(A[i][i]) <= sum)
            A[i][i] += (abs(A[i][i]) - sum - 0.1)*((A[i][i] > 0) ? -1 : 1);
    }
}

template <size_t n>
void print_vector(double x[n]) {
    for (int i=0; i<n; i++) {
        cout << x[i] << " ";
    }

    cout << endl;
}

template <size_t n>
void print_matrix(double A[n][n]) {
    for (int i=0; i<n; i++) {
        print_vector<n>(A[i]);
    }
}

template <size_t n>
void copy_array(double x[n], double y[n]) {
    for (int i=0; i<n; i++) {
        x[i] = y[i];
    }
}

template <size_t n>
void jacobi_method(double A[n][n], double b[n], double eps, double res[n]) {
    int k = 0;

    double x_start[n] = {0,0,0};
    double x_k[n];
    double x_prev[n];
    copy_array<n>(x_k, x_start);

    while ( true ) {
        copy_array<n>(x_prev, x_k);
        for (int i=0; i<n; i++) {
            double sigma = 0;
            for (int j=0; j<n; j++) {
                if  ( j!=i )
                    sigma = sigma + A[i][j]*x_prev[j];
            }
            x_k[i] = (b[i] - sigma)/A[i][i];
        }
        auto dist = euclidean_distance<n>(x_k, x_prev);
        //print_vector<n>(x_prev);
        //print_vector<n>(x_k);
        cout << dist << endl;
        if (k == 100) {
            cout << "WARNING: out of iterations !!" << endl;
            break;
        }
        if (dist <= 1.0e-6)
            break;
        k ++;
    }

    copy_array<n>(res, x_k);
}

int main() {
    srand(time(NULL));
    cout << "SEQUENTIAL JACOBI METHOD" << endl;
    const int n = 3;
    double A[n][n];
    double b[n];
    double res[n];
    initialize_matrix_A<n>(A, b, -10, 10);
    cout << int(is_diagonally_dominant<n>(A)) << endl;
    print_matrix<n>(A);
    print_vector<n>(b);
    {
        auto timer = utimer("jacobi with n = 4");

        jacobi_method<n>(A, b, 0.000000001, res);
    }

    print_vector<n>(res);
}
