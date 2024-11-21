#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>
#include <cstdlib>
#include <ctime>

#include <mpi.h>

#include "../config.hpp"

int main(int argc, char** argv) {
    // Initialize MPI
    MPI_Init(&argc, &argv);

    // Get group size and our position
    MPI_Comm comm = MPI_COMM_WORLD;
    int rank, size;

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    // Generate array and distribute
    const size_t ARRAY_SIZE = 1000000;
    std::vector<int> array;
    if (rank == 0) {
        array.resize(ARRAY_SIZE);
        std::srand(std::time(0));
        for (size_t i = 0; i < ARRAY_SIZE; ++i) {
            array[i] = std::rand() % 5000000 + 1;
        }
    }

    size_t local_size = ARRAY_SIZE / size;
    std::vector<int> local_array(local_size);
    MPI_Scatter(array.data(), local_size, MPI_INT, local_array.data(), local_size, MPI_INT, 0, comm);

    // Get the element to search for
    int target;
    if (rank == 0) {
        std::cout << "Enter the element to search for: ";
        std::cin >> target;
    }
    MPI_Bcast(&target, 1, MPI_INT, 0, comm);

    // Start timer
    MPI_Barrier(comm);
    timer::time_point start = timer::now();

    // Search for the element
    int local_index = -1;
    for (size_t i = 0; i < local_size; ++i) {
        if (local_array[i] == target) {
            local_index = i;
            break;
        }
    }

    // Gather results
    int global_index = -1;
    if (local_index != -1) {
        global_index = rank * local_size + local_index;
    }
    MPI_Allreduce(MPI_IN_PLACE, &global_index, 1, MPI_INT, MPI_MAX, comm);

    // Stop timer
    MPI_Barrier(comm);
    timer::time_point end = timer::now();

    // Print results only on rank 0
    if (rank == 0) {
        if (global_index != -1) {
            std::cout << "Element found at index: " << global_index << std::endl;
        } else {
            std::cout << "Element not found" << std::endl;
        }
        std::cout << "Time: " << get_time_ms(end - start) << " seconds" << std::endl;
    }

    // Finish up MPI
    MPI_Finalize();
    return 0;
}
