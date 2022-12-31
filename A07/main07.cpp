#include <iostream>
#include <math.h> 
#include "mpi.h"

void prt(float* p, int n){
    for (int i = 0; i < n; i++) std::cout << *(p + i) << " ";
    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    int numprocs, myid, R, C = 5;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    if (myid == 0) {
        std::cout << "specify row number:" << std::endl;
        // std::cin >> R;
        R = 5;
        int* cont;
        float data[R][C];
        data[0][0] = -1; data[0][1] = 0; data[0][2] = 1; data[0][3] = 2; data[0][4] = 3; 
        for (int r = 1; r < R; r++) {
            data[r][0] = pow(data[0][0], r);
            data[r][1] = 0;
            data[r][2] = 1;
            data[r][3] = pow(data[0][3], -r);
            data[r][4] = pow(data[0][4], -r);
        }
        for (int i = 0; i < R; i++) {
            *cont = 0;
            MPI_Send(&cont, 1, MPI_INT, i % (numprocs - 1) + 1, 0, MPI_COMM_WORLD);
            MPI_Send(data[i], 5, MPI_FLOAT, i % (numprocs - 1) + 1, 1, MPI_COMM_WORLD);
        }
        for (int i = 0; i < numprocs; i++) {
            *cont = 1;
            MPI_Send(cont, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        int cont;
        float message[5];
        while (true) {
            MPI_Recv(&cont, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            // std::cout << "process " << myid << " receive cont " << cont << std::endl;
            if (cont == 1) break;
            MPI_Recv(message, 5, MPI_FLOAT, 0, 1, MPI_COMM_WORLD, &status);
            std::cout << "process " << myid << " receive data ";
            prt(message, 5);
        };
    }
    MPI_Finalize();
    return 0;
} /* end main */