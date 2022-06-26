#include <iostream>
#include <cmath>
#include <random>
#include <vector>

#include "utils.hpp"

using namespace std;

// Computing the euclidan distance between two vectors x and y
float euclidean_distance(vector<float> x, vector<float> y) {
    int n = x.size();
    float dist = 0;

    for (int i=0; i<n; i++) {
        dist += pow((x[i] - y[i]), 2);
    }

    return sqrt(dist);
}

// Return a random number taked uniformelly in the range [minimum, maximum]
float random_in_range(int minimum, int maximum) {
    auto randValue = rand() / float(RAND_MAX); // random number in [0, 1]
    randValue = randValue*(maximum - minimum) + minimum;

    return randValue;
}

// Check if the matrix A is diagonally dominant
bool is_diagonally_dominant(vector<vector<float>> A) {
    int n = A.size();

    for (int i=0; i<n; i++) {
        float sum = 0;
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

// Initialize the problem appropiatelly filling the vectors A and b
void initialize_problem(vector<vector<float>> &A, vector<float> &b, int minimum, int maximum) {
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
        float sum = 0;
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

// Utility that print a vector
void print_vector(vector<float> b) {
    for (auto i: b) {
        cout << i << " ";
    }
    cout << endl;
}

// Utility that print a matrix
void print_matrix(vector<vector<float>> A) {
    for (int i=0; i<A.size(); i++) {
        for (int j=0; j<A[i].size(); j++)
            std::cout << A[i][j] << ' ';
        cout << endl;
    }
    cout << endl;
}
