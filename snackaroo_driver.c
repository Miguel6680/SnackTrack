/*
-snackaroo_driver.c-
Miguel Mallet
*/ 

/*
implementation of driver management functions
*/

#include "snackaroo_driver.h"

//convert colour int 0–6 to string for printing
static const char *colour_to_string(int c)
{
    switch (c) {
        case COLOUR_RED:   return "red";
        case COLOUR_GREEN: return "green";
        case COLOUR_BLUE:  return "blue";
        case COLOUR_GREY:  return "grey";
        case COLOUR_WHITE: return "white";
        case COLOUR_BLACK: return "black";
        case COLOUR_OTHER: return "other";
        default:           return "unknown";
    }
}

//licence plate validation, 2–8 chars, alnum or space only 
static int plate_valid(const char *plate)
{
    int len = (int) strlen(plate);
    if (len < 2 || len > MAX_PLATE) return 0;

    int i;
    for (i = 0; i < len; i++) {
        unsigned char ch = (unsigned char) plate[i];
        if (!(isalnum(ch) || ch == ' ')) {
            return 0;
        }
    }
    return 1;
}

//find driver by code or NULL
Driver *driver_find(Driver *head, int code)
{
    Driver *cur = head;
    while (cur != NULL) {
        if (cur->code == code) return cur;
        cur = cur->next;
    }
    return NULL;
}

//insert new driver 
int driver_insert(Driver **head)
{
    int code, colour;
    char name[MAX_DRIVER_NAME + 1];
    char plate[MAX_PLATE + 1];

    if (!read_int("\tEnter driver code: ", &code)) {
        printf("Invalid input.\n");
        return 0;
    }
    if (code < 0) {
        printf("Driver code must be positive.\n");
        return 0;
    }
    if (driver_find(*head, code) != NULL) {
        printf("Driver already exists.\n");
        return 0;
    }

    read_line("\tEnter driver name: ", name, sizeof(name));

    if (!read_int("\tEnter vehicle colour (0=red,1=green,2=blue,3=grey,4=white,5=black,6=other): ",
                  &colour)) {
        printf("Invalid input.\n");
        return 0;
    }
    if (colour < 0 || colour > 6) {
        printf("Invalid colour.\n");
        return 0;
    }

    read_line("\tEnter licence plate: ", plate, sizeof(plate));
    if (!plate_valid(plate)) {
        printf("Licence plate must be 2–8 characters, alphanumeric or space only.\n");
        return 0;
    }

    Driver *node = (Driver *) malloc(sizeof(Driver));
    if (node == NULL) {
        printf("Database full. Could not allocate memory.\n");
        return 0;
    }

    node->code = code;
    strncpy(node->name, name, MAX_DRIVER_NAME);
    node->name[MAX_DRIVER_NAME] = '\0';
    node->colour = colour;
    strncpy(node->plate, plate, MAX_PLATE);
    node->plate[MAX_PLATE] = '\0';

    node->next = *head;
    *head = node;

    return 1;
}

//update driver
int driver_update(Driver *head)
{
    int code, colour;
    char name[MAX_DRIVER_NAME + 1];
    char plate[MAX_PLATE + 1];

    if (!read_int("\tEnter driver code: ", &code)) {
        printf("Invalid input.\n");
        return 0;
    }

    Driver *d = driver_find(head, code);
    if (d == NULL) {
        printf("Driver not found.\n");
        return 0;
    }

    read_line("\tEnter driver name: ", name, sizeof(name));

    if (!read_int("\tEnter vehicle colour (0=red,1=green,2=blue,3=grey,4=white,5=black,6=other): ",
                  &colour)) {
        printf("Invalid input.\n");
        return 0;
    }
    if (colour < 0 || colour > 6) {
        printf("Invalid colour.\n");
        return 0;
    }

    read_line("\tEnter licence plate: ", plate, sizeof(plate));
    if (!plate_valid(plate)) {
        printf("Licence plate must be 2–8 characters, alphanumeric or space only.\n");
        return 0;
    }

    strncpy(d->name, name, MAX_DRIVER_NAME);
    d->name[MAX_DRIVER_NAME] = '\0';
    d->colour = colour;
    strncpy(d->plate, plate, MAX_PLATE);
    d->plate[MAX_PLATE] = '\0';

    return 1;
}

//delete driver, enforcing referential integrity
int driver_delete(Driver **head, Relationship *rel_head)
{
    int code;

    if (!read_int("\tEnter driver code: ", &code)) {
        printf("Invalid input.\n");
        return 0;
    }

    //check if any relationship refers to this driver
    Relationship *rel = rel_head;
    while (rel != NULL) {
        if (rel->driver_code == code) {
            printf("Cannot erase driver: one or more relationships exist.\n");
            printf("Remove the relationships first.\n");
            return 0;
        }
        rel = rel->next;
    }

    Driver *cur = *head;
    Driver *prev = NULL;

    while (cur != NULL && cur->code != code) {
        prev = cur;
        cur = cur->next;
    }

    if (cur == NULL) {
        printf("Driver not found.\n");
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

//print a single driver 
void driver_print_one(const Driver *d)
{
    if (d == NULL) return;

    printf("%-11d %-20s %-13s %-8s\n",
           d->code, d->name, colour_to_string(d->colour), d->plate);
}

//print all drivers
void driver_print_all(Driver *head)
{
    printf("Driver Code Driver Name           Vehicle Colour License\n");

    Driver *cur = head;
    while (cur != NULL) {
        driver_print_one(cur);
        cur = cur->next;
    }
}

//free the entire driver list
void driver_free_all(Driver **head)
{
    Driver *cur = *head;
    while (cur != NULL) {
        Driver *next = cur->next;
        free(cur);
        cur = next;
    }
    *head = NULL;
}

// dump / restore

int driver_dump(Driver *head)
{
    char filename[256];
    FILE *fp;
    Driver *cur;

    read_line("\tEnter output file: ", filename, sizeof(filename));
    fp = fopen(filename, "wb");
    if (fp == NULL) {
        printf("Could not open file for writing.\n");
        return 0;
    }

    cur = head;
    while (cur != NULL) {
        if (fwrite(cur, sizeof(Driver), 1, fp) != 1) {
            printf("Error writing to file.\n");
            fclose(fp);
            return 0;
        }
        cur = cur->next;
    }

    fclose(fp);
    return 1;
}

int driver_restore(Driver **head)
{
    char filename[256];
    FILE *fp;
    Driver temp;

    read_line("\tEnter input file: ", filename, sizeof(filename));
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Could not open file for reading.\n");
        return 0;
    }

    driver_free_all(head);

    while (fread(&temp, sizeof(Driver), 1, fp) == 1) {
        Driver *node = (Driver *) malloc(sizeof(Driver));
        if (node == NULL) {
            printf("Database full. Could not allocate memory.\n");
            fclose(fp);
            return 0;
        }
        *node = temp;
        node->next = *head;
        *head = node;
    }

    fclose(fp);
    return 1;
}
