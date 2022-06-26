#include <vector>

using namespace std;

#ifndef UTILS_H
#define UTILS_H

// Computing the euclidan distance between two vectors x and y
float euclidean_distance(vector<float> x, vector<float> y);

// Return a random number taked uniformelly in the range [minimum, maximum]
float random_in_range(int minimum, int maximum);

// Check if the matrix A is diagonally dominant
bool is_diagonally_dominant(vector<vector<float>> A);

// Initialize the problem appropiatelly filling the vectors A and b
void initialize_problem(vector<vector<float>> &A, vector<float> &b, int minimum, int maximum);

// Utility that print a vector
void print_vector(vector<float> b);

// Utility that print a matrix
void print_matrix(vector<vector<float>> A);

#endif
