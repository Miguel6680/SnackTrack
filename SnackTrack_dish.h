/*
-SnackTrack_dish.h-
Miguel Mallet
*/ 

/*
prototypes for dish management functions
*/

#ifndef SNACKTRACK_DISH_H
#define SNACKTRACK_DISH_H

#include "SnackTrack.h"

//operations on dish linked list
Dish *dish_find(Dish *head, int code);
int   dish_insert(Dish **head);
int   dish_update(Dish *head);
int   dish_delete(Dish **head, Relationship *rel_head); 
void  dish_print_one(const Dish *d);
void  dish_print_all(Dish *head);
void  dish_free_all(Dish **head);

// dump/restore
int   dish_dump(Dish *head);
int   dish_restore(Dish **head);

#endif
