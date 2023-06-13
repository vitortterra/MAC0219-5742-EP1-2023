CC     = gcc
CFLAGS = -Wall -pedantic -O2 -std=c11

lca: lca.c
	$(CC) $(CFLAGS) -o $@ $^