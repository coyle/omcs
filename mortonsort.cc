#ifdef _OPENMP
#include <parallel/algorithm>
#include <parallel/numeric>
#else
#include <algorithm>
#endif

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <getopt.h>
#include "MortonKeyCalculator.hh"

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

#define USAGE                                                                 \
"usage:\n"                                                                    \
"  driver [options]\n"                                                        \
"options:\n"                                                                  \
"  -n [nbodies]        Number of bodies (Default: 0.1)\n"                     \
"  -h                  Show this help message\n" 

int main(int argc, char **argv){
	int nbodies = 10;
	double max_range = 2.0e-5;
	int seed = 5;
	char *filename = NULL;
	int option_char;

	while ((option_char = getopt(argc, argv, "o:n:s:r:h")) != -1) {
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
		case 'r':
			max_range = atof(optarg);
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

	Body *bodies = new Body[nbodies];

	srand(seed);
	makeCluster(bodies, nbodies, 0.0, 0.0, max_range);

	MortonKeyCalculator mkcalc(bodies, nbodies);
	#ifdef _OPENMP
	__gnu_parallel::sort(bodies, bodies + nbodies, mkcalc);
	#else
	std::sort(bodies, bodies + nbodies, mkcalc);
	#endif

	if(!filename){
		for(int i = 0; i < nbodies; i++)
			mkcalc.printKey(cout, bodies[i]);
	}
	else{
		std::ofstream os(filename);
		for(int i = 0; i < nbodies; i++)
			mkcalc.printKey(os, bodies[i]);	
	}

	delete[] bodies;
	return EXIT_SUCCESS;
}

