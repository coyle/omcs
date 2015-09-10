#ifndef _FORCEBARNESHUT_HH_
#define _FORCEBARNESHUT_HH_

#include "ForceCalculator.hh"
#include "Body.hh"

class ForceBarnesHut : public ForceCalculator {

	public:
		ForceBarnesHut(Body *body, int N, double theta);

		virtual void operator() (Body *pulled);

		virtual ~ForceBarnesHut(){};

	private:
		Body *bodies_;
		const int N_;
		double theta_;
};

#endif