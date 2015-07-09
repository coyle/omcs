#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#include "broadcast_test.h"
#include "test_utils.h"

struct broadcast_instance_t{
  void *buffer;
  int count;
  MPI_Datatype datatype;
  int root;
  MPI_Comm comm;

  void* ref_buffer;
};

broadcast_instance_t* broadcast_instance_create(int count){
  broadcast_instance_t* self = (broadcast_instance_t*) malloc(sizeof(broadcast_instance_t));


  self->count = count * getP();
  self->datatype = MPI_INT;
  self->root = 0;
  self->comm = MPI_COMM_WORLD;

  self->buffer = malloc(self->count * sizeof(int));
  self->ref_buffer = malloc(self->count * sizeof(int));

  broadcast_instance_reset(self);

  MPI_Bcast(self->ref_buffer, self->count, self->datatype, 
            self->root, self->comm);

  return self;
}

void broadcast_instance_destroy(broadcast_instance_t* self){
  free(self->buffer);
  free(self->ref_buffer);
  free(self);
}

void broadcast_instance_reset(broadcast_instance_t* self){
  int* buf = (int*) self->buffer;
  int* ref_buf = (int*) self->ref_buffer;

  if (getRank() != 0){
    for(int i = 0; i < self->count; i++){
      buf[i] = 0;
      ref_buf[i] = buf[i];
    }
  }
  else{
    for(int i = 0; i < self->count; i++){
      buf[i] = i;
      ref_buf[i] = buf[i];
    }
  }
}

int broadcast_instance_call(broadcast_instance_t* self, 
                           int (*broadcast)(void*, int, MPI_Datatype, int, MPI_Comm)){
  return broadcast(self->buffer, self->count, self->datatype, self->root, self->comm);
}

int broadcast_instance_fails(broadcast_instance_t* self, 
                           int (*broadcast)(void *buffer, int count, MPI_Datatype sendtype,
                                          int root, MPI_Comm comm)){
  int local_result, global_result;

  broadcast(self->buffer, self->count, self->datatype, 
            self->root, self->comm);

  local_result = (memcmp(self->buffer, self->ref_buffer, self->count * sizeof(int)) != 0);

  MPI_Allreduce(&local_result, &global_result, 1, MPI_INT, MPI_LOR, self->comm);

  return global_result;
}
