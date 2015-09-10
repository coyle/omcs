#include <cstdlib>
#include <iostream>
#include <fstream>
#include <getopt.h>
#include "Integrator.hh"
#include "ForceNaive.hh"
#include "ForceBarnesHut.hh"

using namespace std;

class CSVOutput : public OnStepComplete{
	private:
		ostream& fd_;
		bool isempty_;
		int count;
		const int every;
	public:
		CSVOutput(ostream& fd, int every) : fd_(fd), isempty_(true), count(0), every(every){}

		virtual void operator() (UniverseState *state){
			if(isempty_){
				fd_<<"time_idx,body_idx,t,x,y,vx,vy,m"<<endl;
				isempty_ = false;
			}
			if (count % every ==0)
				writeToCSV(fd_, *state, count / every);

			//cerr<<count<<endl;
			count++;
		}

		virtual ~CSVOutput(){}
};

UniverseState* loadState(const char *filename){
	ifstream is;

	is.open(filename);

	if(is.fail()){
		cerr<<"Opening "<<filename<<" failed."<<endl;
		exit(EXIT_FAILURE);
	}

	return UniverseState::createFromFile(is);
};

class MyForceCalculatorFactory : public ForceCalculatorFactory{
	public:
		MyForceCalculatorFactory() : use_barneshut_(false), theta_(0.0) {}
		MyForceCalculatorFactory(double theta) : use_barneshut_(true), theta_(theta){}

		virtual ForceCalculator* operator()(Body *bodies, int N){
			if(use_barneshut_)
				return new ForceBarnesHut(bodies, N, theta_);
			else	
				return new ForceNaive(bodies, N);
		}

	private:
		const bool use_barneshut_;
		const double theta_;
};

#define USAGE                                                                 \
"usage:\n"                                                                    \
"  driver [options]\n"                                                        \
"options:\n"                                                                  \
"  -f [filename]       Initial conditions (Default: initial.us)\n"           \
"  -n [nsteps]         Number of steps (Default: 60000)\n"                    \
"  -r [nsteps]         Record once every nsteps (Default: 1000)\n"            \
"  -s [step_size]      Earth days in step (Default: 0.003)\n"                 \
"  -t [theta]          Barnes hut distance ratio parameter (Default: 0.1)\n"  \
"  -b                  Use Barneshut rather than naive (Default: 0.1)\n"      \
"  -h                  Show this help message\n" 

int main(int argc, char **argv){
	const char *filename = "initial.us";
	int nsteps = 60000; 
	int r = 1000;
	double h = 0.003;
	double theta = 0.1;
	bool use_barneshut = false;
	int option_char;

	while ((option_char = getopt(argc, argv, "f:n:r:s:t:bh")) != -1) {
		switch (option_char) {
		case 'f':
			filename = optarg;
			break;
		case 'n':
			nsteps = atoi(optarg);
			break;
		case 'r':
			r = atoi(optarg);
			break;
		case 's':
			h = atof(optarg);
			break;
		case 't':
			theta = atof(optarg);
			break;
		case 'b':
			use_barneshut = true;
			break;
		case 'h':
			fprintf(stderr, "%s\n", USAGE);
			exit(EXIT_SUCCESS);
			break;                      
		default:
			fprintf(stderr, "%s\n", USAGE);
			exit(EXIT_FAILURE);
		}
	}

	UniverseState* state = loadState(filename);
	CSVOutput csvoutput(cout, r);
	MyForceCalculatorFactory *fcfactory;
	if(use_barneshut)
		fcfactory = new MyForceCalculatorFactory(theta);
	else
		fcfactory = new MyForceCalculatorFactory();

	Integrator integrator(state, &csvoutput, fcfactory);

	integrator.run(h, nsteps);

	delete fcfactory;
	delete state;
}

