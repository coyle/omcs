#include "ForceNaive.hh"
#include "Timer.hh"

ForceNaive::ForceNaive(Body *bodies, int N): 
	force_time(0.0),
	bodies_(bodies),
	N_(N)
{}

void ForceNaive::operator()(Body *pulled){
	Timer force_timer;
	force_timer.start();

	for(int i = 0; i < N_; i++)
		bodies_[i].pulls(pulled);

	force_time += force_timer.stop();
};
