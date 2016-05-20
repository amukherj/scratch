all: rwlock_test

clean:
	rm -f *.o rwloack_test

CFLAGS=-g -Wall -pthread # -DRWLOCK_WRITEPREF
LDFLAGS=-pthread

rwlock_test: rwlock_test.o rwlock.c
