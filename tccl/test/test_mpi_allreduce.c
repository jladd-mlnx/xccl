/**
* Copyright (C) Mellanox Technologies Ltd. 2001-2020.  ALL RIGHTS RESERVED.
*
* See file LICENSE for terms.
*/
#include "test_mpi.h"

int main (int argc, char **argv) {
    const int count = 32;
    tccl_coll_req_h request;
    int rank, size, i, status = 0, status_global;
    int sbuf[count], rbuf[count], rbuf_mpi[count];

    tccl_mpi_test_init(argc, argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (i=0; i<count; i++) {
        rbuf[i] = 0;
        sbuf[i] = rank+1+12345 + i;
    }

    tccl_coll_op_args_t coll = {
        .coll_type = TCCL_ALLREDUCE,
        .buffer_info = {
            .src_buffer = sbuf,
            .dst_buffer = rbuf,
            .len        = count*sizeof(int),
        },
        .reduce_info = {
            .dt = TCCL_DT_INT32,
            .op = TCCL_OP_SUM,
            .count = count,
        },
        .alg.set_by_user = 0,
        .tag  = 123, //todo
    };

    tccl_collective_init(&coll, &request, tccl_world_team);
    tccl_collective_post(request);
    tccl_collective_wait(request);
    tccl_collective_finalize(request);

    MPI_Allreduce(sbuf, rbuf_mpi, count, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    if (0 != memcmp(rbuf, rbuf_mpi, count*sizeof(int))) {
        fprintf(stderr, "RST CHECK FAILURE at rank %d\n", rank);
        status = 1;
    }

    MPI_Reduce(&status, &status_global, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    if (0 == rank) {
        printf("Correctness check: %s\n", status_global == 0 ? "PASS" : "FAIL");
    }

    tccl_mpi_test_finalize();
    return 0;
}
