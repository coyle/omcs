#include <cstdlib>
#include <sstream>
#include <iostream>
#include "UniverseState.hh"


UniverseState::UniverseState(int nbodies) : 
	nbodies_(nbodies),
	t_(0.0){

	bodies_ = new Body[nbodies_ ? nbodies_  : 1];
}

UniverseState::~UniverseState(){
	delete [] bodies_;
}

UniverseState* UniverseState::createFromFile(istream &is){
	int nbodies;
	double t;

	is>>nbodies;
	UniverseState* ans = new UniverseState(nbodies);

	is>>t;
	ans->set_t(t);

	Body *bodies = ans->bodies();

	for(int i = 0; i < nbodies; i++){
		double x,y,vx,vy,m;
		is>>x>>y>>vx>>vy>>m;
		bodies[i] = Body(x,y,vx,vy,m);
	}

	return ans;
}

std::ostream& writeToFile(ostream& os, const UniverseState& state){
	os<<state.nbodies()<<endl;
	os<<state.t()<<endl;

	const int N = state.nbodies();
	Body *b = state.bodies();
	for(int i = 0 ; i < N; i++){
		os<<b[i].x()<<" "
		  <<b[i].y()<<" "
  		  <<b[i].vx()<<" "
		  <<b[i].vy()<<" "
		  <<b[i].m()<<endl;
	}

	return os;
}

std::ostream& writeToCSV(std::ostream& os, const UniverseState& state, int outer_idx){
	double t = state.t();

	Body *bodies = state.bodies();

	for(int i = 0; i < state.nbodies(); i++)
		os<<outer_idx
		  <<","<<i
		  <<","<<t
		  <<","<<bodies[i].x()
		  <<","<<bodies[i].y()
		  <<","<<bodies[i].vx()
		  <<","<<bodies[i].vy()
		  <<","<<bodies[i].m()<<endl;

	return os;
}
