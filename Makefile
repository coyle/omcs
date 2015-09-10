CXX = g++
FLAGS = -O3 -g -Wall -std=c++0x -fopenmp

nbody: nbody.cc  Body.cc Integrator.cc UniverseState.cc ForceBarnesHut.cc ForceNaive.cc
	$(CXX) $(FLAGS) -o $@ $^

mortonsort: mortonsort.cc Body.cc MortonKeyCalculator.cc
	$(CXX) $(FLAGS) -o $@ $^

benchmark: benchmark.cc Body.cc ForceBarnesHut.cc 
	$(CXX) $(FLAGS) -o $@ $^

CLEANFILES = nbody mortonsort benchmark
CLEANFILES += *.o

clean:
	rm -f *~ $(CLEANFILES)