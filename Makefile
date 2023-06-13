CC     = gcc
CFLAGS = -Wall -pedantic -O2 -std=c11

lca: lca.c common.o
	$(CC) $(CFLAGS) -o $@ $<

grid_gen: grid_gen.c common.o
	$(CC) $(CFLAGS) -o $@ $<

common.o: common.h
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm *.o lca grid_gen
