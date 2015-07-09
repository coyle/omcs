#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>
#include <string.h>

#include "test_utils.h"
#include "reduce_test.h"

struct reduce_instance_t{
  void *sendbuf;
  void *recvbuf;
  int count;
  MPI_Datatype datatype;
  MPI_Op op;
  int root;
  MPI_Comm comm;

  void *ref_recvbuf;
};


reduce_instance_t* reduce_instance_create(int count){
  reduce_instance_t* self = (reduce_instance_t*) malloc(sizeof(reduce_instance_t));

  self->sendbuf = malloc(count * sizeof(int));

  if(getRank() != 0){
    self->recvbuf = NULL;
    self->ref_recvbuf = NULL;
  }
  else{
    self->recvbuf = malloc(count * sizeof(int));
    self->ref_recvbuf = malloc(count * sizeof(int));
  }

  self->count = count;
  self->datatype = MPI_INT;
  self->op = MPI_SUM;
  self->root = 0;
  self->comm = MPI_COMM_WORLD;

  reduce_instance_reset(self);

  MPI_Reduce(self->sendbuf, self->ref_recvbuf, self->count, 
             self->datatype, self->op, self->root, self->comm);

  return self;
}

void reduce_instance_destroy(reduce_instance_t* self){
  free(self->sendbuf);

  if (getRank() == 0){
    free(self->recvbuf);
    free(self->ref_recvbuf);
  }

  free(self);
}

void reduce_instance_reset(reduce_instance_t* self){
  int *buf = (int*) self->sendbuf;

  for(int i = 0; i < self->count; i++)
    buf[i] = i;

  if (getRank() == 0)
    memset(self->recvbuf, 0, self->count * sizeof(int));
}

int reduce_instance_call(reduce_instance_t* self, 
                          int (*reduce)(const void *, void *, 
                                        int, MPI_Datatype, MPI_Op,
                                        int, MPI_Comm)){
  return reduce(self->sendbuf, self->recvbuf, 
         self->count, self->datatype, self->op,
         self->root, self->comm);
}

int reduce_instance_fails(reduce_instance_t* self, 
                          int (*reduce)(const void *, void *, 
                                        int, MPI_Datatype, MPI_Op,
                                        int, MPI_Comm)){
  int local_result, global_result;

  reduce(self->sendbuf, self->recvbuf, 
         self->count, self->datatype, self->op,
         self->root, self->comm);

  if(getRank() != 0)
    local_result = 0;
  else 
    local_result = (memcmp(self->recvbuf, self->ref_recvbuf, self->count * sizeof(int)) != 0);

  MPI_Allreduce(&local_result, &global_result, 1, MPI_INT, MPI_LOR, self->comm);

  return global_result;
}



