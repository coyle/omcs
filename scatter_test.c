#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <assert.h>

#include "scatter_test.h"
#include "test_utils.h"

struct scatter_instance_t{
  void *sendbuf;
  int sendcount;
  MPI_Datatype sendtype;
  void *recvbuf;
  int recvcount;
  MPI_Datatype recvtype;
  int root;
  MPI_Comm comm;

  void *ref_recvbuf;

};


scatter_instance_t* scatter_instance_create(int count){
  scatter_instance_t* self = (scatter_instance_t*) malloc(sizeof(scatter_instance_t));

  if(getRank() != 0)
    self->sendbuf = NULL;
  else
    self->sendbuf = malloc(count * sizeof(int) * getP());

  self->sendcount = count;
  self->sendtype = MPI_INT;

  self->recvbuf = malloc(count * sizeof(int));
  self->recvcount = count;
  self->recvtype = MPI_INT;
  self->root = 0;
  self->comm = MPI_COMM_WORLD;

  self->ref_recvbuf = malloc(count * sizeof(int));

  scatter_instance_reset(self);

  MPI_Scatter(self->sendbuf, self->sendcount, self->sendtype, 
      self->ref_recvbuf, self->recvcount, self->recvtype,
      self->root, self->comm);

  return self;
}

void scatter_instance_destroy(scatter_instance_t* self){

  if (getRank() == 0)
    free(self->sendbuf);
  free(self->recvbuf);
  free(self->ref_recvbuf);
  free(self);
}

void scatter_instance_reset(scatter_instance_t* self){
  int *buf = (int*) self->sendbuf;

  if (getRank() == 0)
    for(int i = 0; i < self->sendcount * getP(); i++)
      buf[i] = i;

  memset(self->recvbuf, 0, self->recvcount * sizeof(int));
}

int scatter_instance_call(scatter_instance_t* self, 
                         int (*scatter)(void *, int , MPI_Datatype,
                                       void *, int,  MPI_Datatype, 
                                       int , MPI_Comm )){
  return scatter(self->sendbuf, self->sendcount, self->sendtype, 
    self->recvbuf, self->recvcount, self->recvtype,
    self->root, self->comm);
}

int scatter_instance_fails(scatter_instance_t* self, 
                           int (*scatter)(void *, int, MPI_Datatype,
                                          void *, int, MPI_Datatype, 
                                          int, MPI_Comm)){
  int local_result, global_result;

  scatter(self->sendbuf, self->sendcount, self->sendtype, 
      self->recvbuf, self->recvcount, self->recvtype,
      self->root, self->comm);

  local_result = (memcmp(self->recvbuf, self->ref_recvbuf, self->recvcount * sizeof(int)) != 0);

  MPI_Allreduce(&local_result, &global_result, 1, MPI_INT, MPI_LOR, self->comm);

  return global_result;
}
