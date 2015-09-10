#ifdef _OPENMP
#include <parallel/algorithm>
#include <parallel/numeric>
#else
#include <algorithm>
#endif

#include "MortonKeyCalculator.hh"


MortonKeyCalculator::MortonKeyCalculator(Body *bodies, int N){
	//Find in parallel the minimum x and y values among bodies and 
	//the range of values (i.e. max(xmax - xmin, ymax - ymin)).

	//You may use stl's min_element and max_element functions if
	//you like.

	//Your code here.
	
};

std::ostream& MortonKeyCalculator::printKey(std::ostream &os, const Body &b) const{
	uint32_t bx = x(b.x());
	uint32_t by = y(b.y());

	for(int i = 31; i >= 0; i--){
		os<<((by >> i) % 2);
		os<<((bx >> i) % 2);
	}
	os<<std::endl;

	return os;
}

bool MortonKeyCalculator::operator() (const Body &a, const Body &b) const{
	//Return true is a is less than b in a Morton Key ordering; 
	//otherwise, return false.


}
