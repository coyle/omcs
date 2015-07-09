#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>
#include <string.h>

#include "test_utils.h"
#include "allgather_test.h"

struct allgather_instance_t{
  void *sendbuf;
  int sendcount;
  MPI_Datatype sendtype;
  void *recvbuf;
  int recvcount;
  MPI_Datatype recvtype;
  MPI_Comm comm;

  void *ref_recvbuf;
};

allgather_instance_t* allgather_instance_create(int count){
  allgather_instance_t* self = (allgather_instance_t*) malloc(sizeof(allgather_instance_t));

  self->sendbuf = malloc(count * sizeof(int));
  
  self->sendcount = count;
  self->sendtype = MPI_INT;

  self->recvbuf = malloc(count * sizeof(int) * getP());
  self->ref_recvbuf = malloc(count * sizeof(int) * getP());
  
  self->recvcount = count;
  self->recvtype = MPI_INT;
  self->comm = MPI_COMM_WORLD;

  allgather_instance_reset(self);

  MPI_Allgather(self->sendbuf, self->sendcount, self->sendtype, 
      self->ref_recvbuf, self->recvcount, self->recvtype, self->comm);

  return self;
}

void allgather_instance_destroy(allgather_instance_t* self){
  free(self->sendbuf);

  free(self->recvbuf);
  free(self->ref_recvbuf);

  free(self);
}

void allgather_instance_reset(allgather_instance_t* self){
  int* buf = (int*) self->sendbuf;

  for(int i = 0; i < self->sendcount; i++)
    buf[i] = i;

  memset(self->recvbuf, 0, self->recvcount * sizeof(int) * getP());
}

int allgather_instance_call(allgather_instance_t* self, 
                           int (*allgather)(void *sendbuf, int sendcount, MPI_Datatype sendtype,
                                          void *recvbuf, int recvcount, MPI_Datatype recvtype, 
                                          MPI_Comm comm)){
  return  allgather(self->sendbuf, self->sendcount, self->sendtype, 
                    self->recvbuf, self->recvcount, self->recvtype,
                    self->comm);
}

int allgather_instance_fails(allgather_instance_t* self, 
                           int (*allgather)(void *sendbuf, int sendcount, MPI_Datatype sendtype,
                                            void *recvbuf, int recvcount, MPI_Datatype recvtype, 
                                            MPI_Comm comm)){
  int local_result, global_result;

  allgather(self->sendbuf, self->sendcount, self->sendtype, 
            self->recvbuf, self->recvcount, self->recvtype,
            self->comm);

  if(getRank() != 0)
    local_result = 0;
  else 
    local_result = (memcmp(self->recvbuf, self->ref_recvbuf, self->recvcount * getP() * sizeof(int)) != 0);

  MPI_Allreduce(&local_result, &global_result, 1, MPI_INT, MPI_LOR, self->comm);

  return global_result;
}
