#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"

#include "products.h"
#include "customers.h"
#include "orders.h"

/* #include "menu_template.h" */
/* #ifndef A2BIS_MENU_TEMPLATE_H */
/* #define A2BIS_MENU_TEMPLATE_H */

/* Prototypes:
 * Since in this example this functions are not used outside menu_template
 * it does not make sense to create an include file and export them
 * BUT in your code it will make sense to break this file in many files
 * and create several include files with the prototypes
 *
 * A static function in C is a function that has a scope that is limited
 * to its object file. This means that the static function is only
 * visible in its object file. Therefore,
 * you need to remove the static keyword from the declaration
 * if the function is declared in a file and used in another
 * */

static int ShowMainMenu();
static void ShowProductsMenu();
static int ShowProductsSubMenu();
static void ShowOrdersMenu();
static int ShowOrdersSubMenu();
static void ShowCustomersMenu();
static int ShowCustomersSubMenu();
static void PrintZub();
static void PrintBigFatGit();
static void PrintAliens();
static void PrintMary();
static void PrintJack();
static void PrintLittleBoPeep();

static void PrintProducts();
static void PrintOrders();
static void PrintCustomers();


/* #endif //A2BIS_MENU_TEMPLATE_H */

/**
 * @file menu_template.c
 * @author rmarabini
 * @date 15 April 2020
 * @brief File containing an example of a menu system with submenus
 *
 * The following program allows a user to select a nursery rhyme
 * or a fairy tail from a menu. The menu has two levels, the top
 * one allows to select between fairy tails or nursery rhymes. The
 * second level (sub-menus) allow to select either a particular
 * rhyme or tail.
 *
 * @see https://bytes.com/topic/c/answers/567136-creating-menus-sub-menus
 */


/**
 * @brief Entry point, shows and process main menu.
 *
 * This function implements a loop that prints the main
 * menu, analyze the user selection and calls the
 * corresponding submenu
 *
 * @return 0 if no error
 * @author rmarabini
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
 *
 *
 * @return selected entry in the menu
 * @author rmarabini
 */

int ShowMainMenu() {
    int nSelected = 0;
    char buf[16]; /* buffer for reading user input
                     in theory we only need 1 char
                     but a large buffer will handle
                     more gracefully wrong inputs
                     as 'qwerty' */

    do {
        printf("This is a nursery rhyme and fairy stories program\n");
        printf("You can choose to display a number of different nursery rhymes and fairy stories\n\n");

        printf(" (1) Products\n"
               " (2) Orders\n"
               " (3) Costumers\n"
               " (4) Exit\n\n"
               "Enter a number that corresponds to your choice > ");
        if (!fgets(buf, 16, stdin))
            /* reading input failed, give up: */
            nSelected =0;
        else
            /* have some input, convert it to integer: */
            nSelected = atoi(buf);
        printf("\n");

        if ((nSelected < 1) || (nSelected > 3)) {
            printf("You have entered an invalid choice. Please try again\n\n\n");
        }
    } while ((nSelected < 1) || (nSelected > 3));

    return nSelected;
}

/**
 * @brief Shows and processes the rhyme menu.
 *
 * This function implements a loop that prints the rhyme
 * menu, analyze the user selection and prints
 * the selected rhyme
 * @return void
 * @author rmarabini
 */

void ShowProductsMenu() {
    int nChoice = 0;
    do {
        nChoice = ShowProductsSubMenu();
        switch (nChoice) {

            case 1: {
                PrintProductsStock();
                /*PrintMary();*/
            }
                break;

            case 2: {
                PrintJack();
            }
                break;

            case 3: {
                printf("Bye Bye\n\n");
            }
                break;

            case 4: {
                
            }
                break;
        }
    } while (nChoice != 4);

}

/**
 * @brief prints rhyme menu and allows to select an option.
 *
 * @return selected option
 * @author rmarabini
 */

 int ShowProductsSubMenu() {
    int nSelected = 0;
    char buf[16];

    do {

        printf(" (1) Stock\n"
               " (2) Find\n"
               " (3) Back\n");

        printf("Enter a number that corresponds to your choice > ");
        if (!fgets(buf, 16, stdin))
            /* reading input failed, give up: */
            nSelected =0;
        else
            /* have some input, convert it to integer: */
            nSelected = atoi(buf);
        printf("\n");


        if ((nSelected < 1) || (nSelected > 3)) {
            printf("You have entered an invalid choice. Please try again\n\n\n");
        }
    } while ((nSelected < 1) || (nSelected > 3));

    return nSelected;
}

/**
 * @brief Shows and processes the fairy menu.
 *
 * This function implements a loop that prints the fairy
 * menu, analyze the user selection and prints
 * the selected fairy tail
 *
 * @return selected option
 * @author rmarabini
 */

void ShowOrdersMenu() {
    int nChoice = 0;

    do {
        nChoice = ShowOrdersSubMenu();
        switch (nChoice) {
            case 1: {
                PrintZub();
            }
                break;

            case 2: {
                PrintBigFatGit();
            }
                break;

            case 3: {
                PrintAliens();
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
 * @brief prints the fairy menu and allows to select an option.
 *
 * @return selected option
 * @author rmarabini
 */
 int ShowOrdersSubMenu() {
    int nSelected = 0;
    char buf[16];

    do {

        printf(" (1) Open\n"
               " (2) Range\n"
               " (3) Detail\n"
               " (4) Back\n\n");

        printf("Enter a number that corresponds to your choice > ");
        if (!fgets(buf, 16, stdin))
            /* reading input failed, give up: */
            nSelected =0;
        else
            /* have some input, convert it to integer: */
            nSelected = atoi(buf);
        printf("\n");

        if ((nSelected < 1) || (nSelected > 4)) {
            printf("You have entered an invalid choice. Please try again\n\n\n");
        }
    } while ((nSelected < 1) || (nSelected > 4));

    return nSelected;
}

/**
 * @brief Shows and processes the fairy menu.
 *
 * This function implements a loop that prints the fairy
 * menu, analyze the user selection and prints
 * the selected fairy tail
 *
 * @return selected option
 * @author rmarabini
 */

void ShowCustomersMenu() {
    int nChoice = 0;

    do {
        nChoice = ShowCustomersSubMenu();
        switch (nChoice) {
            case 1: {
                PrintZub();
            }
                break;

            case 2: {
                PrintBigFatGit();
            }
                break;

            case 3: {
                PrintAliens();
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
 * @brief prints the fairy menu and allows to select an option.
 *
 * @return selected option
 * @author rmarabini
 */
 int ShowCustomersSubMenu() {
    int nSelected = 0;
    char buf[16];

    do {

        printf(" (1) Find\n"
               " (2) List Products\n"
               " (3) Balance\n"
               " (4) Back\n\n");

        printf("Enter a number that corresponds to your choice > ");
        if (!fgets(buf, 16, stdin))
            /* reading input failed, give up: */
            nSelected =0;
        else
            /* have some input, convert it to integer: */
            nSelected = atoi(buf);
        printf("\n");

        if ((nSelected < 1) || (nSelected > 4)) {
            printf("You have entered an invalid choice. Please try again\n\n\n");
        }
    } while ((nSelected < 1) || (nSelected > 4));

    return nSelected;
}
/**
 * @brief prints Rhyme Mary
 *
 * @return void
 * @author rmarabini
 *
 */
 void PrintMary() {
    printf("Mary had a little lamb\n fleese white as snow\n\n\n");
}

/**
 * @brief prints Rhyme Jack
 *
 * @return void
 * @author rmarabini
 *
 */
void PrintJack() {
    printf("Jack and jill went up the hill to fetch water\nThen they fell down\n\n\n");
}

/**
 * @brief prints Rhyme LittleBoPee
 *
 * @return void
 * @author rmarabini
 *
 */
void PrintLittleBoPeep() {
    printf("Little bo peep had sheep\nthen they came home\n\n\n");
}

/**
 * @brief prints fairy tail LittleBoPee
 *
 * @return void
 * @author rmarabini
 *
 */
 void PrintZub() {
    printf("zub zub zub zub zub zub zub zub\n\n\n");
}

/**
 * @brief prints fairy tail LittleBoPee
 *
 * @return void
 * @author rmarabini
 *
 */
void PrintBigFatGit() {
    printf("im big and fat and a git\n\n\n");
}

/**
 * @brief prints fairy tail LittleBoPee
 *
 * @return void
 * @author rmarabini
 *
 */

void PrintAliens() {
    printf("take us to your leader\n\n\n");
}
     
     



/**
 * @brief prints Stock of Products
 *
 * @return void
 * @author Santiago Pita
 *
 */
void PrintProductsStock() {
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    int ret; 
    SQLRETURN ret2; 
    #define BufferLength 512
    char x[BufferLength] = "\0"; /* Para guardar el productcode (input del usuario) */
    char y[BufferLength] = "\0"; /* Para guardar el quantityinstock (resultado) */

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        printf("Error al conectar a la BD\n");
        return;
    }

    /* Allocate a statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        odbc_disconnect(env, dbc);
        return;
    }

    /* Pide el productcode al usuario (como pedía x) */
    printf("Introduce el identificador del producto > ");
    (void) fflush(stdout);

    /* Comprueba que haya input para ejecutar la query */
    if (fgets(x, (int) sizeof(x), stdin) != NULL) {
        char query[BufferLength + 100];

        x[strcspn(x, "\n")] = 0;

        (void) snprintf(query, sizeof(query), "SELECT quantityinstock FROM products WHERE productcode = '%s';", x);

        (void) SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);

        /* Asignamos a 'y' la primera columna seleccionada (quantityinstock) como texto */
        (void) SQLBindCol(stmt, 1, SQL_C_CHAR, (SQLCHAR*) y, BufferLength , NULL);

        printf("\n--- RESULTADO ---\n");
        int encontrado = 0;
        
        while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
            printf("Stock disponible: %s unidades\n", y);
            encontrado = 1;
        }

        if (!encontrado) {
            printf("No se encontro el producto con codigo '%s'.\n", x);
        }
        printf("-----------------\n\n");
        
        ret2 = SQLCloseCursor(stmt);
        if (!SQL_SUCCEEDED(ret2)) {
            odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        }
    }

    /* free up statement handle */
    ret2 = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
    }

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        printf("Error al desconectar de la BD\n");
    }
}