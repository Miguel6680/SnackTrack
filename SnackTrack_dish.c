/*
-SnackTrack_dish.c-
Miguel Mallet
*/ 

/*
implementation of dish management functions
*/

#include "SnackTrack_dish.h"

//find dish with given code or NULL if not found 
Dish *dish_find(Dish *head, int code)
{
    Dish *cur = head;
    while (cur != NULL) {
        if (cur->code == code) return cur;
        cur = cur->next;
    }
    return NULL;
}

//insert a new dish 
int dish_insert(Dish **head)
{
    int code;
    float rating, price;
    char name[MAX_DISH_NAME + 1];
    char restaurant[MAX_RESTAURANT + 1];

    if (!read_int("\tEnter dish code: ", &code)) {
        printf("Invalid input.\n");
        return 0;
    }
    if (code < 0) {
        printf("Dish code must be positive.\n");
        return 0;
    }
    if (dish_find(*head, code) != NULL) {
        printf("Dish already exists.\n");
        return 0;
    }

    read_line("\tEnter dish name: ", name, sizeof(name));
    read_line("\tEnter restaurant name: ", restaurant, sizeof(restaurant));

    if (!read_float("\tEnter dish rating: ", &rating)) {
        printf("Invalid input.\n");
        return 0;
    }
    if (rating < 0.0f || rating > 10.0f) {
        printf("Rating must be between 0.0 and 10.0\n");
        return 0;
    }

    if (!read_float("\tEnter dish price: ", &price)) {
        printf("Invalid input.\n");
        return 0;
    }
    if (price <= 0.0f) {
        printf("Price must be positive.\n");
        return 0;
    }

    Dish *node = (Dish *) malloc(sizeof(Dish));
    if (node == NULL) {
        printf("Database full. Could not allocate memory.\n");
        return 0;
    }

    node->code = code;
    strncpy(node->name, name, MAX_DISH_NAME);
    node->name[MAX_DISH_NAME] = '\0';
    strncpy(node->restaurant, restaurant, MAX_RESTAURANT);
    node->restaurant[MAX_RESTAURANT] = '\0';
    node->rating = rating;
    node->price  = price;

    //insert at front of list
    node->next = *head;
    *head = node;

    return 1;
}

//update existing dish
int dish_update(Dish *head)
{
    int code;
    float rating, price;
    char name[MAX_DISH_NAME + 1];
    char restaurant[MAX_RESTAURANT + 1];

    if (!read_int("\tEnter dish code: ", &code)) {
        printf("Invalid input.\n");
        return 0;
    }

    Dish *d = dish_find(head, code);
    if (d == NULL) {
        printf("Dish not found.\n");
        return 0;
    }

    read_line("\tEnter dish name: ", name, sizeof(name));
    read_line("\tEnter restaurant name: ", restaurant, sizeof(restaurant));

    if (!read_float("\tEnter dish rating: ", &rating)) {
        printf("Invalid input.\n");
        return 0;
    }
    if (rating < 0.0f || rating > 10.0f) {
        printf("Rating must be between 0.0 and 10.0\n");
        return 0;
    }

    if (!read_float("\tEnter dish price: ", &price)) {
        printf("Invalid input.\n");
        return 0;
    }
    if (price <= 0.0f) {
        printf("Price must be positive.\n");
        return 0;
    }

    strncpy(d->name, name, MAX_DISH_NAME);
    d->name[MAX_DISH_NAME] = '\0';
    strncpy(d->restaurant, restaurant, MAX_RESTAURANT);
    d->restaurant[MAX_RESTAURANT] = '\0';
    d->rating = rating;
    d->price  = price;

    return 1;
}

//delete a dish
int dish_delete(Dish **head, Relationship *rel_head)
{
    int code;

    if (!read_int("\tEnter dish code: ", &code)) {
        printf("Invalid input.\n");
        return 0;
    }

    //check if any relationship refers to this dish
    Relationship *rel = rel_head;
    while (rel != NULL) {
        if (rel->dish_code == code) {
            printf("Cannot erase dish: one or more relationships exist.\n");
            printf("Remove the relationships first.\n");
            return 0;
        }
        rel = rel->next;
    }

    Dish *cur = *head;
    Dish *prev = NULL;

    while (cur != NULL && cur->code != code) {
        prev = cur;
        cur = cur->next;
    }

    if (cur == NULL) {
        printf("Dish not found.\n");
        return 0;
    }

    if (prev == NULL) {          
        *head = cur->next;
    } else {
        prev->next = cur->next;
    }

    free(cur);
    return 1;
}

//prints a single dish
void dish_print_one(const Dish *d)
{
    if (d == NULL) return;

    printf("%-11d %-25s %-30s %-10.1f $%5.2f\n",
           d->code, d->name, d->restaurant, d->rating, d->price);
}

//prints all dishes as a table
void dish_print_all(Dish *head)
{
    printf("Dish Code   Dish Name                 Restaurant Name                 Dish Rating   Dish Price\n");

    Dish *cur = head;
    while (cur != NULL) {
        dish_print_one(cur);
        cur = cur->next;
    }
}

//dree entire dish list 
void dish_free_all(Dish **head)
{
    Dish *cur = *head;
    while (cur != NULL) {
        Dish *next = cur->next;
        free(cur);
        cur = next;
    }
    *head = NULL;
}

// dump / restore using fwrite / fread

int dish_dump(Dish *head)
{
    char filename[256];
    FILE *fp;
    Dish *cur;

    read_line("\tEnter output file: ", filename, sizeof(filename));
    fp = fopen(filename, "wb");
    if (fp == NULL) {
        printf("Could not open file for writing.\n");
        return 0;
    }

    cur = head;
    while (cur != NULL) {
        if (fwrite(cur, sizeof(Dish), 1, fp) != 1) {
            printf("Error writing to file.\n");
            fclose(fp);
            return 0;
        }
        cur = cur->next;
    }

    fclose(fp);
    return 1;
}

int dish_restore(Dish **head)
{
    char filename[256];
    FILE *fp;
    Dish temp;

    read_line("\tEnter input file: ", filename, sizeof(filename));
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Could not open file for reading.\n");
        return 0;
    }

    //clear current list
    dish_free_all(head);

    while (fread(&temp, sizeof(Dish), 1, fp) == 1) {
        Dish *node = (Dish *) malloc(sizeof(Dish));
        if (node == NULL) {
            printf("Database full. Could not allocate memory.\n");
            fclose(fp);
            return 0;
        }
        *node = temp;       //copy all fields 
        node->next = *head; //rebuild next pointer
        *head = node;
    }

    fclose(fp);
    return 1;
}
