#include <iostream>
#include <cmath>
#include "utimer.cpp"
#include <random>
#include <vector>

using namespace std;

double euclidean_distance(vector<double> x, vector<double> y) {
    int n = x.size();
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

bool is_diagonally_dominant(vector<vector<double>> A) {
    int n = A.size();

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

void initialize_problem(vector<vector<double>> &A, vector<double> &b, int minimum, int maximum) {
    int n = A.size();

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

void print_vector(vector<double> b) {
    for (auto i: b) {
        cout << i << " ";
    }
    cout << endl;
}

void jacobi_method(vector<vector<double>> &A, vector<double> &b, double eps, vector<double> &res) {
    int k = 0;

    int n = A.size();

    vector<double> x_start = {0,0,0};
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

        auto dist = euclidean_distance(x_k, x_prev);

        cout << dist << endl;

        if (k == 100) {
            cout << "WARNING: out of iterations !!" << endl;
            break;
        }
        if (dist <= 1.0e-6)
            break;
        k ++;
    }

    res = x_k;
}

void print_matrix(vector<vector<double>> A) {
    for (int i=0; i<A.size(); i++) {
        for (int j=0; j<A[i].size(); j++)
            std::cout << A[i][j] << ' ';
        cout << endl;
    }
    cout << endl;
}

int main() {
    srand(time(NULL));
    cout << "SEQUENTIAL JACOBI METHOD" << endl;
    const int n = 3;

    // instantiate matrix A
    vector<vector<double>> A(n);
    for (int i=0; i<n; i++) {
        A[i] = vector<double>(n);
    }

    // instantiate vector b
    vector<double> b(n);

    vector<double> res(n);
    initialize_problem(A, b, -10, 10);

    print_matrix(A);
    print_vector(b);

    cout << int(is_diagonally_dominant(A)) << endl;
    {
        auto timer = utimer("jacobi with n = 4");

        jacobi_method(A, b, 0.000000001, res);
    }

    print_vector(res);
}
