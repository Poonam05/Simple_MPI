#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Size of the array
    int n = 100;
    int *array = NULL;
    int local_sum = 0;

    if (world_rank == 0) {
        // Master process initializes the array
        array = (int *)malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) {
            array[i] = i + 1;  // Array elements are 1, 2, 3, ..., n
        }
    }

    // Determine the portion of the array each process will handle
    int elements_per_proc = n / world_size;
    int *sub_array = (int *)malloc(elements_per_proc * sizeof(int));

    // Scatter the array to all processes
    MPI_Scatter(array, elements_per_proc, MPI_INT, sub_array, elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

    // Each process calculates its local sum
    for (int i = 0; i < elements_per_proc; i++) {
        local_sum += sub_array[i];
    }

    // Reduce all local sums to a global sum in the root process
    int global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        printf("The total sum of the array is %d\n", global_sum);
        free(array);
    }

    free(sub_array);
    MPI_Finalize();
    return 0;
}
