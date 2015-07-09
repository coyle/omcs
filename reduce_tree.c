#include <assert.h>
#include <mpi.h>

int GT_Reduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype,
	MPI_Op op, int root, MPI_Comm comm){

	assert(datatype == MPI_INT);
	assert(op == MPI_SUM);
	assert(root == 0);

	/* Your code here */

}

