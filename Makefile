CC     = gcc
CFLAGS = -Wall -pedantic -O2 -std=c11

all: lca grid_gen

lca: lca.c common.h
	$(CC) $(CFLAGS) -o $@ $<

grid_gen: grid_gen.c common.h
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm lca grid_gen
