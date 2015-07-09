#ifndef _GATHER_H_
#define _GATHER_H_


typedef struct gather_instance_t gather_instance_t;

gather_instance_t* gather_instance_create(int count);
void gather_instance_destroy(gather_instance_t* self);
void gather_instance_reset(gather_instance_t* self);

int gather_instance_call(gather_instance_t* self, 
                         int (*gather)(void *, int , MPI_Datatype,
                                       void *, int,  MPI_Datatype, 
                                       int , MPI_Comm ));

int gather_instance_fails(gather_instance_t* self, 
                          int (*gather)(void *, int , MPI_Datatype,
                                        void *, int,  MPI_Datatype, 
                                        int , MPI_Comm ));
#endif