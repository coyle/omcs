#include <cmath>
#include "Body.hh"

const double Body::G = 8.88508e-10; //AU^3 day^{-2} M_earth^{-1}
const double Body::min_dist = 3.0E4/149597871000.0; //AU

void Body::pulls(Body *that) const{
	double dx = this->x_ - that->x_;
	double dy = this->y_ - that->y_;
	
	double dist = sqrt(dx * dx + dy * dy);

	//Preventing a division by zero and a body from pulling itself.
	if(dist > 0){
		double F = G * that->m_ * this->m_ / (dist * dist + min_dist * min_dist);
		that->fx_ += F * dx / dist;
		that->fy_ += F * dy / dist;
	}
}

void Body::integrate(double h){
	vx_ += h * fx_ / m_;
	vy_ += h * fy_ / m_;
	x_ += h * vx_;
	y_ += h * vy_;
	fx_ = 0.0;
	fy_ = 0.0;
};



