# Makefile for 2211 SnackTrack

CC     = gcc
CFLAGS = -Wall -std=c99

OBJS = SnackTrack.o SnackTrack_dish.o SnackTrack_driver.o SnackTrack_rel.o

SnackTrack: $(OBJS)
	$(CC) $(CFLAGS) -o SnackTrack $(OBJS)

SnackTrack.o: SnackTrack.c SnackTrack.h SnackTrack_dish.h SnackTrack_driver.h SnackTrack_rel.h
	$(CC) $(CFLAGS) -c SnackTrack.c

SnackTrack_dish.o: SnackTrack_dish.c SnackTrack_dish.h SnackTrack.h
	$(CC) $(CFLAGS) -c SnackTrack_dish.c

SnackTrack_driver.o: SnackTrack_driver.c SnackTrack_driver.h SnackTrack.h
	$(CC) $(CFLAGS) -c SnackTrack_driver.c

SnackTrack_rel.o: SnackTrack_rel.c SnackTrack_rel.h SnackTrack.h SnackTrack_dish.h SnackTrack_driver.h
	$(CC) $(CFLAGS) -c SnackTrack_rel.c

clean:
	rm -f *.o SnackTrack
