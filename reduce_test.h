#ifndef _REDUCE_TEST_H_
#define _REDUCE_TEST_H_

#include <mpi.h>

typedef struct reduce_instance_t reduce_instance_t;

reduce_instance_t* reduce_instance_create(int count);
void reduce_instance_destroy(reduce_instance_t* self);
void reduce_instance_reset(reduce_instance_t* self);
int reduce_instance_call(reduce_instance_t* self, 
                          int (*reduce)(const void *, void *, 
                                        int, MPI_Datatype, MPI_Op,
                                        int, MPI_Comm));
int reduce_instance_fails(reduce_instance_t* self, 
                          int (*reduce)(const void *, void *, 
                                        int, MPI_Datatype, MPI_Op,
                                        int, MPI_Comm));

#endif