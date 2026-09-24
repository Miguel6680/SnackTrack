/*
-SnackTrack_driver.h-
Miguel Mallet
*/ 

/*
prototypes for driver management functions
*/

#ifndef SNACKTRACK_DRIVER_H
#define SNACKTRACK_DRIVER_H

#include "SnackTrack.h"

Driver *driver_find(Driver *head, int code);
int     driver_insert(Driver **head);
int     driver_update(Driver *head);
int     driver_delete(Driver **head, Relationship *rel_head); //checks referential integrity
void    driver_print_one(const Driver *d);
void    driver_print_all(Driver *head);
void    driver_free_all(Driver **head);

// dump/restore 
int     driver_dump(Driver *head);
int     driver_restore(Driver **head);

#endif
