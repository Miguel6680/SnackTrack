/*
-snackaroo.c-
Miguel Mallet
*/ 

/*
main program for 2211 Snackaroo 
*/

#include "snackaroo.h"
#include "snackaroo_dish.h"
#include "snackaroo_driver.h"
#include "snackaroo_rel.h"

//input helpers
//read a whole line of any length. Store up to size-1 chars and discard extra
void read_line(const char *prompt, char *buffer, int size)
{
    int ch;
    size_t len;

    printf("%s", prompt);

    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }

    len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else {
        //too long, flush remainder of line 
        while ((ch = getchar()) != '\n' && ch != EOF) {
            // discard 
        }
    }
}

//read an int using a line buffer to avoid scanf issues
int read_int(const char *prompt, int *out)
{
    char line[128];

    printf("%s", prompt);
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return 0;
    }
    if (sscanf(line, "%d", out) != 1) {
        return 0;
    }
    return 1;
}

//read a float using a line buffer
int read_float(const char *prompt, float *out)
{
    char line[128];

    printf("%s", prompt);
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return 0;
    }
    if (sscanf(line, "%f", out) != 1) {
        return 0;
    }
    return 1;
}

//help and sub menus
void print_help(void)
{
    printf("Main commands:\n");
    printf("  h - print this help message\n");
    printf("  m - manage dishes (insert/search/update/print/erase/dump/restore)\n");
    printf("  a - manage drivers (insert/search/update/print/erase/dump/restore)\n");
    printf("  r - manage dish/driver relationships\n");
    printf("  q - quit (all data is lost)\n");
}

//dish submenu: i, s, u, p, e, d, r, q
void dish_menu(Dish **head, Relationship *rel_head)
{
    char op;
    int running = 1;
    int ch;

    while (running) {
        printf("\nDish operations: i=insert, s=search, u=update, p=print, e=erase, d=dump, r=restore, q=return\n");
        printf("Enter dish operation code: ");
        if (scanf(" %c", &op) != 1) return;

        //flush rest of line so read_line/read_int work properly
        while ((ch = getchar()) != '\n' && ch != EOF) { }

        switch (op) {
            case 'i':
                dish_insert(head);
                break;

            case 's': {
                int code;
                Dish *d;

                if (!read_int("\tEnter dish code: ", &code)) {
                    printf("Invalid input.\n");
                    break;
                }
                d = dish_find(*head, code);
                if (d == NULL) {
                    printf("Dish not found.\n");
                } else {
                    printf("Dish Code   Dish Name                 Restaurant Name                 Dish Rating   Dish Price\n");
                    dish_print_one(d);
                }
                break;
            }

            case 'u':
                dish_update(*head);
                break;

            case 'p':
                dish_print_all(*head);
                break;

            case 'e':
                dish_delete(head, rel_head);
                break;

            case 'd':
                dish_dump(*head);
                break;

            case 'r':
                dish_restore(head);
                break;

            case 'q':
                running = 0;   //return to main menu 
                break;

            default:
                printf("Illegal dish operation code.\n");
                break;
        }
    }
}

//driver submenu: i, s, u, p, e, d, r, q 
void driver_menu(Driver **head, Relationship *rel_head)
{
    char op;
    int running = 1;
    int ch;

    while (running) {
        printf("\nDriver operations: i=insert, s=search, u=update, p=print, e=erase, d=dump, r=restore, q=return\n");
        printf("Enter driver operation code: ");
        if (scanf(" %c", &op) != 1) return;

        //flush rest of line
        while ((ch = getchar()) != '\n' && ch != EOF) { }

        switch (op) {
            case 'i':
                driver_insert(head);
                break;

            case 's': {
                int code;
                Driver *d;

                if (!read_int("\tEnter driver code: ", &code)) {
                    printf("Invalid input.\n");
                    break;
                }
                d = driver_find(*head, code);
                if (d == NULL) {
                    printf("Driver not found.\n");
                } else {
                    printf("Driver Code Driver Name           Vehicle Colour License\n");
                    driver_print_one(d);
                }
                break;
            }

            case 'u':
                driver_update(*head);
                break;

            case 'p':
                driver_print_all(*head);
                break;

            case 'e':
                driver_delete(head, rel_head);
                break;

            case 'd':
                driver_dump(*head);
                break;

            case 'r':
                driver_restore(head);
                break;

            case 'q':
                running = 0;   //return to main menu 
                break;

            default:
                printf("Illegal driver operation code.\n");
                break;
        }
    }
}

//relationship submenu: i,s,p,f,g,x,d,r,q
void relation_menu(Relationship **rel_head, Dish *dish_head, Driver *driver_head)
{
    char op;
    int running = 1;
    int ch;

    while (running) {
        printf("\nRelationship operations:\n");
        printf("  i = insert relationship\n");
        printf("  s = search by relationship code\n");
        printf("  p = print all relationships\n");
        printf("  f = print all dishes for a driver\n");
        printf("  g = print all drivers for a dish\n");
        printf("  x = erase relationship\n");
        printf("  d = dump relationships\n");
        printf("  r = restore relationships\n");
        printf("  q = return to main menu\n");
        printf("Enter relationship operation code: ");

        if (scanf(" %c", &op) != 1) return;

        //flush rest of line
        while ((ch = getchar()) != '\n' && ch != EOF) { }

        switch (op) {
            case 'i':
                rel_insert(rel_head, dish_head, driver_head);
                break;

            case 's': {
                int id;
                Relationship *rel;

                if (!read_int("\tEnter relationship code: ", &id)) {
                    printf("Invalid input.\n");
                    break;
                }
                rel = rel_find(*rel_head, id);
                if (rel == NULL) {
                    printf("Relationship not found.\n");
                } else {
                    printf("Rel Code   Dish Code  Driver Code\n");
                    rel_print_one(rel);
                }
                break;
            }

            case 'p':
                rel_print_all(*rel_head);
                break;

            case 'f':
                rel_print_dishes_for_driver(*rel_head, dish_head);
                break;

            case 'g':
                rel_print_drivers_for_dish(*rel_head, driver_head);
                break;

            case 'x':
                rel_delete(rel_head);
                break;

            case 'd':
                rel_dump(*rel_head);
                break;

            case 'r':
                rel_restore(rel_head, dish_head, driver_head);
                break;

            case 'q':
                running = 0;
                break;

            default:
                printf("Illegal relationship operation code.\n");
                break;
        }
    }
}

//main
int main(void)
{
    Dish         *dish_head   = NULL;
    Driver       *driver_head = NULL;
    Relationship *rel_head    = NULL;
    char op;
    int ch;

    printf("**********************\n");
    printf("* 2211 Snackaroo App *\n");
    printf("**********************\n\n");

    for (;;) {
        printf("\nEnter operation code (h=help, m=dishes, a=drivers, r=relationships, q=quit): ");
        if (scanf(" %c", &op) != 1) {
            break;
        }

        //flush rest of line
        while ((ch = getchar()) != '\n' && ch != EOF) { }

        switch (op) {
            case 'h':
                print_help();
                break;

            case 'm':
                dish_menu(&dish_head, rel_head);
                break;

            case 'a':
                driver_menu(&driver_head, rel_head);
                break;

            case 'r':
                relation_menu(&rel_head, dish_head, driver_head);
                break;

            case 'q':
                dish_free_all(&dish_head);
                driver_free_all(&driver_head);
                rel_free_all(&rel_head);
                return 0;

            default:
                printf("Illegal operation code.\n");
                break;
        }
    }

    //safety cleanup if we ever exit loop unexpectedly
    dish_free_all(&dish_head);
    driver_free_all(&driver_head);
    rel_free_all(&rel_head);
    return 0;
}
