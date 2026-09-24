/*
prototypes for dish/driver relationship management functions
*/

#ifndef SNACKAROO_REL_H
#define SNACKAROO_REL_H

#include "snackaroo.h"
#include "snackaroo_dish.h"
#include "snackaroo_driver.h"

Relationship *rel_find(Relationship *head, int id);
int           rel_insert(Relationship **head, Dish *dish_head, Driver *driver_head);
int           rel_delete(Relationship **head);
void          rel_print_one(const Relationship *rel);
void          rel_print_all(Relationship *head);
void          rel_print_dishes_for_driver(Relationship *head, Dish *dish_head);
void          rel_print_drivers_for_dish(Relationship *head, Driver *driver_head);
void          rel_free_all(Relationship **head);

// dump/restore
int           rel_dump(Relationship *head);
int           rel_restore(Relationship **head, Dish *dish_head, Driver *driver_head);

#endif
