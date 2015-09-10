
#include "Integrator.hh"

Integrator::Integrator(UniverseState *state, 
					   OnStepComplete* onstepcomplete, 
					   ForceCalculatorFactory *fcfactory):
state_(state), 
onstepcomplete_(onstepcomplete), 
fcfactory_(fcfactory)
{}

void Integrator::run(double h, int nsteps){
	const int N = state_->nbodies();
	Body *state_bodies = state_->bodies();
	Body *force_bodies = new Body[N];

	//Outputs initial state
	(*onstepcomplete_)(state_);

	for(int k = 0; k < nsteps; k++){
		#pragma omp parallel for
		for(int i = 0; i < N; i++)
			force_bodies[i] = state_bodies[i];

		ForceCalculator *fc = (*fcfactory_)(force_bodies, N);

		#pragma omp parallel for
		for(int i = 0; i < N; i++)
			(*fc)(&state_bodies[i]);

		#pragma omp parallel for
		for(int i = 0; i < N; i++)
			state_bodies[i].integrate(h);

		state_->set_t(state_->t() + h);

		(*onstepcomplete_)(state_);

		delete fc;
	}

	delete[] force_bodies;
}
