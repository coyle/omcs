#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <getopt.h>
#include <omp.h>
#include "Timer.hh"
#include "ForceBarnesHut.hh"

using namespace std;

void makeCluster(Body *bodies, int N, double cx, double cy, double range){
	for(int i = 0; i < N; i++){
		double x;
		double y;
		do{
		 	x = -1.0 + 2.0 * rand()/(RAND_MAX+1.0);
			y = -1.0 + 2.0 * rand()/(RAND_MAX+1.0);
		}while(x * x + y * y > 1.0);

		x = cx + range * x;
		y = cy + range * y;
		double vx = 0.0;
		double vy = 0.0;
		double m = 1.0e-5;
		bodies[i] = Body(x,y,vx,vy,m);
	}
}

class ForceTest{
	public:
		ForceTest(int nbodies, int nqueries, double theta, unsigned int seed, int max_nthreads) : 
			nbodies_(nbodies),
			nqueries_(nqueries),
			theta_(theta),
			seed_(seed),
			max_nthreads_(max_nthreads)
		{
			double range = 2.0e-5;
			srand(seed);
			omp_set_num_threads(max_nthreads);

			Body *bodies = new Body[nbodies];

			makeCluster(bodies, nbodies, 0.0, 0.0, range);

			queries_ = new Body[nqueries];
			makeCluster(queries_, nqueries, 0.0, 0.0, range);

			//Setting up the reference computation
			Timer timer_setup;
			timer_setup.start();
			ForceBarnesHut f(bodies, nbodies, theta);
			time_setup_ = timer_setup.stop();

			//Running the reference computation
			Timer timer_queries;
			timer_queries.start();
			#pragma omp parallel for	
			for(int i = 0; i < nqueries; i++)
				f(&queries_[i]);
			time_queries_ = timer_queries.stop();

			delete[] bodies;
		}

		std::ostream& printFullJSON(std::ostream &os){
			os<<"{\"nbodies\": "<<nbodies_<<",\n";
			os<<"\"nqueries\": "<<nqueries_<<",\n";
			os<<"\"theta\": "<<theta_<<",\n";
			os<<"\"seed\": "<<seed_<<",\n";
			if(max_nthreads_)
				os<<"\"max_nthreads\": "<<max_nthreads_<<",\n";
			else
				os<<"\"max_nthreads\": null,\n";
			os<<"\"time_setup\": "<<time_setup_<<",\n";
  			os<<"\"time_queries\": "<<time_queries_<<",\n";
  			os<<"\"fx\": [";
  			for(int i = 0; i < nqueries_; i++){
  				os<<queries_[i].fx();
  				if (i+1 < nqueries_)
  					os<<",\n";
  			}
  			os<<"],\n";
  			os<<"\"fy\": [";
  			for(int i = 0; i < nqueries_; i++){
  				os<<queries_[i].fy();
  				if (i+1 < nqueries_)
  					os<<",\n";
  			}
  			os<<"]}\n";

  			return os;
		};

		std::ostream& printSummaryJSON(std::ostream &os){
			os<<"{\"nbodies\": "<<nbodies_<<",\n";
			os<<"\"nqueries\": "<<nqueries_<<",\n";
			os<<"\"theta\": "<<theta_<<",\n";
			os<<"\"seed\": "<<seed_<<",\n";
			if(max_nthreads_)
				os<<"\"max_nthreads\": "<<max_nthreads_<<",\n";
			else
				os<<"\"max_nthreads\": null,\n";
			os<<"\"time_setup\": "<<time_setup_<<",\n";
  			os<<"\"time_queries\": "<<time_queries_<<"}\n";
 
  			return os;
		};

	~ForceTest(){
		delete[] queries_;
	}

	private:
		int nbodies_;
		int nqueries_;
		double theta_;
		unsigned int seed_;
		int max_nthreads_;
		double time_setup_;
		double time_queries_;

		Body *queries_;
};

#define USAGE                                                                 \
"usage:\n"                                                                    \
"  driver [options]\n"                                                        \
"options:\n"                                                                  \
"  -o [filename]       Output file (Default: stdout)\n"                       \
"  -n [nbodies]        Number of bodies (Default: 0.1)\n"                     \
"  -s [seed]           Seed for randomization (Default: 5)\n"                 \
"  -t [theta]          Theta value for BarnesHut (Default: 0.1)\n"            \
"  -q [nqueries]       Number of queries\n"                                   \
"  -u [max_nthreads]   Upper limit on number of threads\n"                    \
"  -h                  Show this help message\n" 

int main(int argc, char **argv){
	int nbodies = 10000;
	double theta = 0.1;
	char *filename = NULL;
	int nqueries = 10000;
	unsigned int seed = 5;
	bool verbose = false;
	int max_nthreads = 0;
	int option_char;

	while ((option_char = getopt(argc, argv, "o:n:s:t:q:u:vh")) != -1) {
		switch (option_char) {
		case 'o':
			filename = optarg;
			break;
		case 'n':
			nbodies = atoi(optarg);
			break;
		case 's':
			seed = atoi(optarg);
			break;
		case 't':
			theta = atof(optarg);
			break;
		case 'q':
			nqueries = atoi(optarg);
			break;
		case 'u':
			max_nthreads = atoi(optarg);
			break;
		case 'v':
			verbose = true;
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

	ForceTest test(nbodies, nqueries, theta, seed, max_nthreads);

	if(filename == NULL)
		if(verbose)
			test.printFullJSON(cout);
		else
			test.printSummaryJSON(cout);
	else{
		ofstream os(filename);
		if(verbose)
			test.printFullJSON(os);
		else
			test.printSummaryJSON(os);
	}

	return EXIT_SUCCESS;
}

