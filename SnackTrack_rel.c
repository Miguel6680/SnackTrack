/*
implementation of dish/driver relationship list (bonus feature)
*/

#include "SnackTrack_rel.h"

//relationship id generator
static int next_rel_id = 0;

//find relationship by id
Relationship *rel_find(Relationship *head, int id)
{
    Relationship *cur = head;
    while (cur != NULL) {
        if (cur->id == id) return cur;
        cur = cur->next;
    }
    return NULL;
}

//insert new relationship 
int rel_insert(Relationship **head, Dish *dish_head, Driver *driver_head)
{
    int dish_code, driver_code;

    if (!read_int("\tEnter dish code: ", &dish_code)) {
        printf("Invalid input.\n");
        return 0;
    }
    if (dish_find(dish_head, dish_code) == NULL) {
        printf("Dish does not exist. Insert the dish first.\n");
        return 0;
    }

    if (!read_int("\tEnter driver code: ", &driver_code)) {
        printf("Invalid input.\n");
        return 0;
    }
    if (driver_find(driver_head, driver_code) == NULL) {
        printf("Driver does not exist. Insert the driver first.\n");
        return 0;
    }

    //prevent duplicate relationships
    Relationship *cur = *head;
    while (cur != NULL) {
        if (cur->dish_code == dish_code && cur->driver_code == driver_code) {
            printf("Relationship already exists.\n");
            return 0;
        }
        cur = cur->next;
    }

    Relationship *node = (Relationship *) malloc(sizeof(Relationship));
    if (node == NULL) {
        printf("Database full. Could not allocate memory.\n");
        return 0;
    }

    node->id          = next_rel_id++;
    node->dish_code   = dish_code;
    node->driver_code = driver_code;

    node->next = *head;
    *head = node;

    return 1;
}

//delete relationship by id
int rel_delete(Relationship **head)
{
    int id;

    if (!read_int("\tEnter relationship code: ", &id)) {
        printf("Invalid input.\n");
        return 0;
    }

    Relationship *cur = *head;
    Relationship *prev = NULL;

    while (cur != NULL && cur->id != id) {
        prev = cur;
        cur = cur->next;
    }

    if (cur == NULL) {
        printf("Relationship not found.\n");
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

//print one relationship
void rel_print_one(const Relationship *rel)
{
    if (rel == NULL) return;

    printf("%-11d %-10d %-10d\n",
           rel->id, rel->dish_code, rel->driver_code);
}

//print all relationships
void rel_print_all(Relationship *head)
{
    printf("Rel Code   Dish Code  Driver Code\n");

    Relationship *cur = head;
    while (cur != NULL) {
        rel_print_one(cur);
        cur = cur->next;
    }
}

//print all dishes delivered by a driver 
void rel_print_dishes_for_driver(Relationship *head, Dish *dish_head)
{
    int driver_code;
    int found = 0;

    if (!read_int("\tEnter driver code: ", &driver_code)) {
        printf("Invalid input.\n");
        return;
    }

    printf("Dish Code   Dish Name                 Restaurant Name                 Dish Rating   Dish Price\n");

    Relationship *cur = head;
    while (cur != NULL) {
        if (cur->driver_code == driver_code) {
            Dish *d = dish_find(dish_head, cur->dish_code);
            if (d != NULL) {
                dish_print_one(d);
                found = 1;
            }
        }
        cur = cur->next;
    }

    if (!found) {
        printf("No dishes found for this driver.\n");
    }
}

//print all drivers that delivered a dish 
void rel_print_drivers_for_dish(Relationship *head, Driver *driver_head)
{
    int dish_code;
    int found = 0;

    if (!read_int("\tEnter dish code: ", &dish_code)) {
        printf("Invalid input.\n");
        return;
    }

    printf("Driver Code Driver Name           Vehicle Colour License\n");

    Relationship *cur = head;
    while (cur != NULL) {
        if (cur->dish_code == dish_code) {
            Driver *d = driver_find(driver_head, cur->driver_code);
            if (d != NULL) {
                driver_print_one(d);
                found = 1;
            }
        }
        cur = cur->next;
    }

    if (!found) {
        printf("No drivers found for this dish.\n");
    }
}

//free all relationships
void rel_free_all(Relationship **head)
{
    Relationship *cur = *head;
    while (cur != NULL) {
        Relationship *next = cur->next;
        free(cur);
        cur = next;
    }
    *head = NULL;
}

// dump / restore

int rel_dump(Relationship *head)
{
    char filename[256];
    FILE *fp;
    Relationship *cur;

    read_line("\tEnter output file: ", filename, sizeof(filename));
    fp = fopen(filename, "wb");
    if (fp == NULL) {
        printf("Could not open file for writing.\n");
        return 0;
    }

    cur = head;
    while (cur != NULL) {
        if (fwrite(cur, sizeof(Relationship), 1, fp) != 1) {
            printf("Error writing to file.\n");
            fclose(fp);
            return 0;
        }
        cur = cur->next;
    }

    fclose(fp);
    return 1;
}

int rel_restore(Relationship **head, Dish *dish_head, Driver *driver_head)
{
    char filename[256];
    FILE *fp;
    Relationship temp;
    int max_id = next_rel_id;

    read_line("\tEnter input file: ", filename, sizeof(filename));
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Could not open file for reading.\n");
        return 0;
    }

    rel_free_all(head);

    while (fread(&temp, sizeof(Relationship), 1, fp) == 1) {
        //enforce referential integrity on restore 
        if (dish_find(dish_head, temp.dish_code) == NULL ||
            driver_find(driver_head, temp.driver_code) == NULL) {
            // skip invalid relationship 
            continue;
        }

        Relationship *node = (Relationship *) malloc(sizeof(Relationship));
        if (node == NULL) {
            printf("Database full. Could not allocate memory.\n");
            fclose(fp);
            return 0;
        }
        *node = temp;
        node->next = *head;
        *head = node;

        if (temp.id >= max_id) {
            max_id = temp.id + 1;
        }
    }

    fclose(fp);
    next_rel_id = max_id;
    return 1;
}
