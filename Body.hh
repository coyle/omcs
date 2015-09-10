#ifndef _BODY_HH_
#define _BODY_HH_

class Body{
	public:
		Body() : m_(0.0) {};
		Body(double x, double y, double vx, double vy, double m):
			x_(x), y_(y), vx_(vx), vy_(vy), m_(m), fx_(0.0), fy_(0.0){};

		double x() const{
			return x_;
		}
		double y() const{
			return y_;
		}
		double vx() const{
			return vx_;
		}
		double vy() const{
			return vy_;
		}
		double m() const{
			return m_;
		}

		double fx() const{
			return fx_;
		}

		double fy() const{
			return fy_;
		}

		//Adds gravitational force due to the body puller.
		void accGravityFrom(Body const &puller);

		//Leapfrog integration
		void integrate(double h);

	private:
		static const double G;
		static const double min_dist;

		double x_;	//x-coordinate
		double y_;	//y-coordinate
		double vx_;	//x velocity
		double vy_;	//y velocity
		double m_;	//mass
		double fx_;
		double fy_;
};

#endif