#ifndef _UNIVERSESTATE_HH_
#define _UNIVERSESTATE_HH_

#include <ostream>
#include <istream>
#include "Body.hh"

using namespace std;

class UniverseState{
	public:
		static UniverseState* createFromFile(istream &is);

		UniverseState(int nbodies);

		//Accessors
		inline int nbodies() const{return nbodies_;};
		inline double t() const{return t_;}
		inline Body* bodies() const{return bodies_;}

		//Mutators
		inline void set_t(double t){t_ = t;}

		~UniverseState();

	private:
		const int nbodies_;		//Number of Bodies
		double t_;				//Current time

		Body *bodies_;			//Bodies
};

std::ostream& writeToFile(ostream& os, const UniverseState& state);
std::ostream& writeToCSV(ostream& os, const UniverseState &state, int outer_idx);

#endif
