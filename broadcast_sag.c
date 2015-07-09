#include <assert.h>
#include <mpi.h>

int GT_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm)
{
  return MPI_Bcast( buffer, count, datatype, root, comm);
}

/* eof */
