#ifndef _FORCENAIVE_HH_
#define _FORCENAIVE_HH_

#include "ForceCalculator.hh"
#include "Body.hh"

class ForceNaive : public ForceCalculator {

	public:
		ForceNaive(Body *bodies, int N);

		virtual void operator() (Body *pulled);

		virtual ~ForceNaive(){};

		double force_time;

	private:
		Body *bodies_;
		const int N_;
};

#endif
