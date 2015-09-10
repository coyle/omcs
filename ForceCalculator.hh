#ifndef _FORCECALCULATOR_HH_
#define _FORCECALCULATOR_HH_

#include "Body.hh"

class ForceCalculator{
	public:

		//Add forces to the body pulled, which may
		//or may not be among the pulling bodies.
		virtual void operator() (Body *pulled) = 0;

		virtual ~ForceCalculator(){};
};

#endif
