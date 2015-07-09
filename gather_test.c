#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <assert.h>

#include "gather_test.h"
#include "test_utils.h"

struct gather_instance_t{
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


gather_instance_t* gather_instance_create(int count){
  gather_instance_t* self = (gather_instance_t*) malloc(sizeof(gather_instance_t));

  self->sendbuf = malloc(count * sizeof(int));
  
  self->sendcount = count;
  self->sendtype = MPI_INT;

  if(getRank() != 0){
    self->recvbuf = NULL;
    self->ref_recvbuf = NULL;
  }
  else{
    self->recvbuf = malloc(count * sizeof(int) * getP());
    self->ref_recvbuf = malloc(count * sizeof(int) * getP());
  }

  self->recvcount = count;
  self->recvtype = MPI_INT;
  self->root = 0;
  self->comm = MPI_COMM_WORLD;

  gather_instance_reset(self);

  MPI_Gather(self->sendbuf, self->sendcount, self->sendtype, 
      self->ref_recvbuf, self->recvcount, self->recvtype,
      self->root, self->comm);

  return self;
}

void gather_instance_destroy(gather_instance_t* self){
  free(self->sendbuf);

  if (getRank() == 0){
    free(self->recvbuf);
    free(self->ref_recvbuf);
  }

  free(self);
}

void gather_instance_reset(gather_instance_t* self){
  int *buf = (int*) self->sendbuf;

  for(int i = 0; i < self->sendcount; i++)
    buf[i] = i;

  if (getRank() == 0)
    memset(self->recvbuf, 0, self->recvcount * sizeof(int) * getP());
}

 int gather_instance_call(gather_instance_t* self, 
                           int (*gather)(void *, int , MPI_Datatype ,
                                          void *, int , MPI_Datatype , 
                                          int , MPI_Comm)){
  return gather(self->sendbuf, self->sendcount, self->sendtype, 
      self->recvbuf, self->recvcount, self->recvtype,
      self->root, self->comm);
}

int gather_instance_fails(gather_instance_t* self, 
                           int (*gather)(void *, int, MPI_Datatype,
                                          void *, int, MPI_Datatype, 
                                          int, MPI_Comm)){
  int local_result, global_result;

  gather(self->sendbuf, self->sendcount, self->sendtype, 
      self->recvbuf, self->recvcount, self->recvtype,
      self->root, self->comm);

  if(getRank() != 0)
    local_result = 0;
  else
    local_result = (memcmp(self->recvbuf, self->ref_recvbuf, self->sendcount * sizeof(int) * getP()) != 0);

  MPI_Allreduce(&local_result, &global_result, 1, MPI_INT, MPI_LOR, self->comm);

  return global_result;
}

