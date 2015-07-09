#include <assert.h>
#include <mpi.h>

int GT_Allgather( void *sendbuf, 
                  int  sendcount,
                  MPI_Datatype sendtype, 
                  void *recvbuf, 
                  int recvcount,
                  MPI_Datatype recvtype, 
                  MPI_Comm comm)
{
	assert(sendtype == MPI_INT && recvtype == MPI_INT);

}
