CC     = gcc
CFLAGS = -Wall -pedantic -O2 -std=c11
CC_OMP = -fopenmp
CC_PTH = -pthread

all: check grid_gen

check: check.o lga_base.o lga_seq.o lga_omp.o lga_pth.o
	$(CC) $(CFLAGS) -o $@ $^

check.o: check.c lga_base.o
	$(CC) $(CFLAGS) -c $<

lga_seq.o: lga_seq.c lga_seq.h lga_base.o
	$(CC) $(CFLAGS) -c $<

lga_omp.o: lga_omp.c lga_omp.h lga_base.o
	$(CC) $(CFLAGS) $(CC_OMP) -c $<

lga_pth.o: lga_pth.c lga_pth.h lga_base.o
	$(CC) $(CFLAGS) $(CC_PTH) -c $<

grid_gen: grid_gen.o lga_base.o
	$(CC) $(CFLAGS) -o $@ $^

grid_gen.o: grid_gen.c lga_base.o
	$(CC) $(CFLAGS) -c $<

lga_base.o: lga_base.c lga_base.h
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm *.o check grid_gen
