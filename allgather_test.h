#ifndef _ALLGATHER_TEST_H_
#define _ALLGATHER_TEST_H_

#include <mpi.h>
#include <assert.h>

#include "test_utils.h"
#include "allgather_test.h"

typedef struct allgather_instance_t allgather_instance_t;

allgather_instance_t* allgather_instance_create(int count);

void allgather_instance_destroy(allgather_instance_t* self);

void allgather_instance_reset(allgather_instance_t* self);

int allgather_instance_call(allgather_instance_t* self, 
                           int (*allgather)(void *sendbuf, int sendcount, MPI_Datatype sendtype,
                                          void *recvbuf, int recvcount, MPI_Datatype recvtype, 
                                          MPI_Comm comm));

int allgather_instance_fails(allgather_instance_t* self, 
                           int (*allgather)(void *sendbuf, int sendcount, MPI_Datatype sendtype,
                                            void *recvbuf, int recvcount, MPI_Datatype recvtype, 
                                            MPI_Comm comm));

#endif