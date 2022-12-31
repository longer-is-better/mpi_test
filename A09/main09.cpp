#include <iostream>
#include <cstdlib>
#include "mpi.h"

void prt(int **data, const int R, const int C){
    for (int r = 0; r < R; r++) {
        for (int c = 0; c < C; c++) {
            std::cout << *((int*)data + r * C + c) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    int numprocs, myid, R, C;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    if (myid == 0) {
        std::cout << "specify row number:" << std::endl;
        // std::cin >> R;
        R = 20;
        // std::cin >> C;
        C = 20;
        int cont;
        int M1[R][C], M2[R][C], M[R][C];
        for (int r = 0; r < R; r++) {
            for (int c = 0; c < C; c++) {
                M1[r][c] = rand() % 10; M2[r][c] = rand() % 10;
            }
        }
        prt((int**)M1, R, C);
        prt((int**)M2, R, C);
        for (int i = 0; i < R; i++) {
            cont = 0;
            MPI_Send(&cont, 1, MPI_INT, i % (numprocs - 1) + 1, 0, MPI_COMM_WORLD);
            std::cout << "sendcount" << std::endl;
            MPI_Send(&C, 1, MPI_INT, i % (numprocs - 1) + 1, 1, MPI_COMM_WORLD);
            std::cout << "sendcount ok" << std::endl;
            MPI_Send(M1[i], C, MPI_INT, i % (numprocs - 1) + 1, 2, MPI_COMM_WORLD);
            MPI_Send(M2[i], C, MPI_INT, i % (numprocs - 1) + 1, 3, MPI_COMM_WORLD);
        }
        for (int i = 0; i < R; i++) {
            cont = 0;
            MPI_Recv(M[i], C, MPI_INT, i % (numprocs - 1) + 1, 4, MPI_COMM_WORLD, &status);
        }
        for (int i = 0; i < numprocs; i++) {
            cont = 1;
            MPI_Send(&cont, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        prt((int**)M, R, C);

    } else {
        int cont;
        while (true) {
            MPI_Recv(&cont, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            if (cont == 1) break;
            std::cout << "process " << myid << " receive cont " << cont << std::endl;
            MPI_Recv(&C, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
            int r_M1[C], r_M2[C], M[C];
            MPI_Recv(r_M1, C, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(r_M2, C, MPI_INT, 0, 3, MPI_COMM_WORLD, &status);
            for (int i = 0; i < C; i++) {
                M[i] = r_M1[i] + r_M2[i];
            }
            MPI_Send(M, C, MPI_INT, 0, 4, MPI_COMM_WORLD);
        };
    }
    MPI_Finalize();
    return 0;
} /* end main */