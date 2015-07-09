#ifndef _SCATTER_H_
#define _SCATTER_H_

typedef struct scatter_instance_t scatter_instance_t;

scatter_instance_t* scatter_instance_create(int count);
void scatter_instance_destroy(scatter_instance_t* self);
void scatter_instance_reset(scatter_instance_t* self);

int scatter_instance_call(scatter_instance_t* self, 
                         int (*scatter)(void *, int , MPI_Datatype,
                                       void *, int,  MPI_Datatype, 
                                       int , MPI_Comm ));

int scatter_instance_fails(scatter_instance_t* self, 
                          int (*scatter)(void *, int , MPI_Datatype,
                                        void *, int,  MPI_Datatype, 
                                        int , MPI_Comm ));
#endif