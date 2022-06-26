#include <vector>

using namespace std;

#ifndef UTILS_H
#define UTILS_H

// Computing the euclidan distance between two vectors x and y
double euclidean_distance(vector<double> x, vector<double> y);

// Return a random number taked uniformelly in the range [minimum, maximum]
double random_in_range(int minimum, int maximum);

// Check if the matrix A is diagonally dominant
bool is_diagonally_dominant(vector<vector<double>> A);

// Initialize the problem appropiatelly filling the vectors A and b
void initialize_problem(vector<vector<double>> &A, vector<double> &b, int minimum, int maximum);

// Utility that print a vector
void print_vector(vector<double> b);

// Utility that print a matrix
void print_matrix(vector<vector<double>> A);

#endif
