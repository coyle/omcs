MPICC = mpicc
MPICFLAGS = -std=c99
MPICOPTFLAGS = -O2 -g -Wall
MPILDFLAGS =

EXEEXT =

IMPL = tree
COLLECTIVE = reduce

IMPL_LC = $(shell echo $(IMPL) | tr A-Z a-z )
COLLECTIVE_LC = $(shell echo $(COLLECTIVE) | tr A-Z a-z )

COLLECTIVE_SRC = $(COLLECTIVE_LC)_$(IMPL_LC).c $(COLLECTIVE_LC)_test.c
DRIVERMACRO = DRIVE$(shell echo $(COLLECTIVE) | tr a-z A-Z)


driver$(EXEXEXT) : driver.c $(COLLECTIVE_SRC) test_utils.c
	$(MPICC) $(MPICFLAGS) $(MPICOPTFLAGS) -D$(DRIVERMACRO) -o $@ $^ $(MPILDFLAGS)

.PHONY: clean

clean:
	rm -f *.o driver