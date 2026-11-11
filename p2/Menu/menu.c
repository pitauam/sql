#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"

#include "products.h"
#include "customers.h"
#include "orders.h"
#define BufferLength 512

/* Prototypes */
static int ShowMainMenu(void);
static void ShowProductsMenu(void);
static int ShowProductsSubMenu(void);
static void ShowOrdersMenu(void);
static int ShowOrdersSubMenu(void);
static void ShowCustomersMenu(void);
static int ShowCustomersSubMenu(void);

int PrintOpen(void);
int PrintRange(void);
int PrintDetail(void);
int PrintStock(void);

/**
 * @brief Entry point, shows and process main menu.
 */
int main(void) {
    int nChoice = 0;

    do {
        nChoice = ShowMainMenu();
        switch (nChoice) {
            case 1: {
                ShowProductsMenu();
            }
                break;

            case 2: {
                ShowOrdersMenu();
            }
                break;

            case 3: {
                ShowCustomersMenu();
            }
                break;
            case 4: {
                printf("Bye Bye\n\n");
            }
                break;
        }
    } while (nChoice != 4);
    return 0;
}


/**
 * @brief prints main menu and allows to select an option.
 */
int ShowMainMenu(void) {
    int nSelected = 0;
    char buf[16];

    do {
        printf("This is a program where you can execute different querys\n");
        printf("You can choose a number of different queries\n\n");

        printf(" (1) Products\n"
               " (2) Orders\n"
               " (3) Costumers\n"
               " (4) Exit\n\n"
               "Enter a number that corresponds to your choice > ");
        if (!fgets(buf, (int)sizeof(buf), stdin))
            nSelected = 0;
        else
            nSelected = atoi(buf);
        printf("\n");

        if ((nSelected < 1) || (nSelected > 4)) {
            printf("You have entered an invalid choice. Please try again\n\n\n");
        }
    } while ((nSelected < 1) || (nSelected > 4));

    return nSelected;
}


/**
 * @brief Shows and processes the products menu.
 */
void ShowProductsMenu(void) {
    int nChoice = 0;
    do {
        nChoice = ShowProductsSubMenu();
        switch (nChoice) {

            case 1: {
                (void)PrintStock();
            }
                break;

            case 2: {
                (void)PrintFindProducts();
            }
                break;

            case 3: {
                printf("Bye Bye\n\n");
            }
                break;

        }
    } while (nChoice != 3);
}


/**
 * @brief prints products submenu and allows to select an option.
 */
int ShowProductsSubMenu(void) {
    int nSelected = 0;
    char buf[16];

    do {
        printf(" (1) Stock\n"
               " (2) Find\n"
               " (3) Back\n");

        printf("Enter a number that corresponds to your choice > ");
        if (!fgets(buf, (int)sizeof(buf), stdin))
            nSelected = 0;
        else
            nSelected = atoi(buf);
        printf("\n");

        if ((nSelected < 1) || (nSelected > 3)) {
            printf("You have entered an invalid choice. Please try again\n\n\n");
        }
    } while ((nSelected < 1) || (nSelected > 3));

    return nSelected;
}


/**
 * @brief Shows and processes the orders menu.
 */
void ShowOrdersMenu(void) {
    int nChoice = 0;

    do {
        nChoice = ShowOrdersSubMenu();
        switch (nChoice) {
            case 1: {
                (void)PrintOpen();
            }
                break;

            case 2: {
                (void)PrintRange();
            }
                break;

            case 3: {
                (void)PrintDetail();
            }
                break;

            case 4: {
                printf("Bye Bye\n\n");
            }
                break;
        }
    } while (nChoice != 4);
}


/**
 * @brief prints the orders submenu and allows to select an option.
 */
int ShowOrdersSubMenu(void) {
    int nSelected = 0;
    char buf[16];

    do {
        printf(" (1) Open\n"
               " (2) Range\n"
               " (3) Detail\n"
               " (4) Back\n\n");

        printf("Enter a number that corresponds to your choice > ");
        if (!fgets(buf, (int)sizeof(buf), stdin))
            nSelected = 0;
        else
            nSelected = atoi(buf);
        printf("\n");

        if ((nSelected < 1) || (nSelected > 4)) {
            printf("You have entered an invalid choice. Please try again\n\n\n");
        }
    } while ((nSelected < 1) || (nSelected > 4));

    return nSelected;
}


/**
 * @brief Shows and processes the customers menu.
 */
void ShowCustomersMenu(void) {
    int nChoice = 0;

    do {
        nChoice = ShowCustomersSubMenu();
        switch (nChoice) {
            case 1: {
                (void)PrintFindCustomers();
            }
                break;

            case 2: {
                (void)PrintListProducts();
            }
                break;

            case 3: {
                (void)PrintBalance();
            }
                break;

            case 4: {
                printf("Bye Bye\n\n");
            }
                break;
        }
    } while (nChoice != 4);
}


/**
 * @brief prints the customers submenu and allows to select an option.
 */
int ShowCustomersSubMenu(void) {
    int nSelected = 0;
    char buf[16];

    do {
        printf(" (1) Find\n"
               " (2) List Products\n"
               " (3) Balance\n"
               " (4) Back\n\n");

        printf("Enter a number that corresponds to your choice > ");
        if (!fgets(buf, (int)sizeof(buf), stdin))
            nSelected = 0;
        else
            nSelected = atoi(buf);
        printf("\n");

        if ((nSelected < 1) || (nSelected > 4)) {
            printf("You have entered an invalid choice. Please try again\n\n\n");
        }
    } while ((nSelected < 1) || (nSelected > 4));

    return nSelected;
}