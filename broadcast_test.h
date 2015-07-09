#ifndef _BROADCAST_TEST_H
#define _BROADCAST_TEST_H

#include <mpi.h>

typedef struct broadcast_instance_t broadcast_instance_t;

broadcast_instance_t* broadcast_instance_create(int count);
void broadcast_instance_destroy(broadcast_instance_t* self);
void broadcast_instance_reset(broadcast_instance_t* self);
int broadcast_instance_call(broadcast_instance_t* self, 
                           int (*broacast)(void*, int, MPI_Datatype, int, MPI_Comm));
int broadcast_instance_fails(broadcast_instance_t* self, 
                           int (*broacast)(void *buffer, int count, MPI_Datatype sendtype,
                                          int root, MPI_Comm comm));

#endif
