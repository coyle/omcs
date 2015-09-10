#include "ForceNaive.hh"
#include "Timer.hh"

ForceNaive::ForceNaive(Body *bodies, int N): 
	force_time(0.0),
	bodies_(bodies),
	N_(N)
{}

void ForceNaive::operator()(Body *pulled){

	for(int i = 0; i < N_; i++)
		pulled->accGravityFrom(bodies_[i]);

};
