/*
-snackaroo.h-
Miguel Mallet
*/ 

/*
definitions and prototypes for snackaroo 
*/

#ifndef SNACKAROO_H
#define SNACKAROO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//constants
#define MAX_DISH_NAME      99   
#define MAX_RESTAURANT     99
#define MAX_DRIVER_NAME    49   
#define MAX_PLATE          8    

//vehicle colour codes
#define COLOUR_RED    0
#define COLOUR_GREEN  1
#define COLOUR_BLUE   2
#define COLOUR_GREY   3
#define COLOUR_WHITE  4
#define COLOUR_BLACK  5
#define COLOUR_OTHER  6

typedef struct Dish Dish;
typedef struct Driver Driver;
typedef struct Relationship Relationship;

//dish node
struct Dish {
    int   code;                               
    char  name[MAX_DISH_NAME + 1];            
    char  restaurant[MAX_RESTAURANT + 1];     
    float rating;                             
    float price;                              
    Dish *next;
};

//driver node
struct Driver {
    int   code;                               
    char  name[MAX_DRIVER_NAME + 1];          
    int   colour;                             
    char  plate[MAX_PLATE + 1];               
    Driver *next;
};

//relationship node
/* 
 * A relationship that says: driver X delivered dish Y.
 */

struct Relationship {
    int id;           //relationship code
    int dish_code;    
    int driver_code;  
    Relationship *next;
};

//input helpers
void read_line(const char *prompt, char *buffer, int size);
int  read_int(const char *prompt, int *out);
int  read_float(const char *prompt, float *out);

//menu helpers
void print_help(void);
void dish_menu(Dish **dish_head, Relationship *rel_head);
void driver_menu(Driver **driver_head, Relationship *rel_head);
void relation_menu(Relationship **rel_head, Dish *dish_head, Driver *driver_head);

#endif
