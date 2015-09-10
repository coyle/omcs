#include "ForceBarnesHut.hh"
#include "Timer.hh"

ForceBarnesHut::ForceBarnesHut(Body *bodies, int N, double theta): 
	bodies_(bodies),
	N_(N),
	theta_(theta)
{
	//Build the quad tree and do any other needed initialization here.
}

void ForceBarnesHut::operator()(Body *pulled){

	//Replace the naive code below with an efficient implementation
	//of Barnes-Hut.
	for(int i = 0; i < N_; i++)
		pulled->accGravityFrom(bodies_[i]);
};
