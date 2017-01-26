#ifndef _INTEGRATOR_HH_
#define _INTEGRATOR_HH_

#include "UniverseState.hh"
#include "ForceCalculator.hh"

class ForceCalculatorFactory{
	public:
		virtual ForceCalculator* operator()(Body *bodies, int N) = 0;
		virtual ~ForceCalculatorFactory(){};
};

class OnStepComplete {
	public:
		virtual void operator() (UniverseState *state) = 0;
		virtual ~OnStepComplete(){};
};

class Integrator {
	public:
		Integrator(UniverseState *state, OnStepComplete* onstepcomplete, ForceCalculatorFactory *fcfactory);

		void run(double h, int nsteps);

	private:
		UniverseState *state_;
		OnStepComplete* onstepcomplete_;
		ForceCalculatorFactory* fcfactory_;
};

#endif
