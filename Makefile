# Makefile for 2211 snackaroo

CC     = gcc
CFLAGS = -Wall -std=c99

OBJS = snackaroo.o snackaroo_dish.o snackaroo_driver.o snackaroo_rel.o

snackaroo: $(OBJS)
	$(CC) $(CFLAGS) -o snackaroo $(OBJS)

snackaroo.o: snackaroo.c snackaroo.h snackaroo_dish.h snackaroo_driver.h snackaroo_rel.h
	$(CC) $(CFLAGS) -c snackaroo.c

snackaroo_dish.o: snackaroo_dish.c snackaroo_dish.h snackaroo.h
	$(CC) $(CFLAGS) -c snackaroo_dish.c

snackaroo_driver.o: snackaroo_driver.c snackaroo_driver.h snackaroo.h
	$(CC) $(CFLAGS) -c snackaroo_driver.c

snackaroo_rel.o: snackaroo_rel.c snackaroo_rel.h snackaroo.h snackaroo_dish.h snackaroo_driver.h
	$(CC) $(CFLAGS) -c snackaroo_rel.c

clean:
	rm -f *.o snackaroo
