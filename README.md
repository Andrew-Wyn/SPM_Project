# SPM_Project
Parallel and distributed system project  

Jacobi Iterative Method Implementation

## Usage 
```
main dimension_of_the_problem number_of_worker max_number_of_iteration use_distance_stop_criteria
```

The parameters:
- dimension_of_the_problem (int).
- number_of_worker (int).
- max_number_of_iteration (int).
- use_distance_stop_criteria (0|1).

To execute the code with the debug utilities, so printing the distances computed the matrices, the vectors and other stuffs, set the environment variable DEBUG=1.

## Compile
To compile the program use the make command,
- ```make all``` to clean and compile.
- ```make main``` to compile.
- ```make clean``` to remove the previously created executable.