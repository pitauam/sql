#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utils.h"

#define BufferLength 512
#define MENU_NAME_LENGTH 256

/* Prototypes */
static int ShowMainMenu(void);
static void ShowProductsMenu(void);
static int ShowProductsSubMenu(void);
static void ShowOrdersMenu(void);
static int ShowOrdersSubMenu(void);
static void ShowCustomersMenu(void);
static int ShowCustomersSubMenu(void);

static char selectedTable[MENU_NAME_LENGTH];
static char selectedIndex[MENU_NAME_LENGTH];
static bool tableSelected = false;

static void trimLine(char *text) {
    size_t len;

    if (!text) return;
    len = strlen(text);
    while (len > 0 && (text[len - 1] == '\n' || text[len - 1] == '\r')) {
        text[--len] = '\0';
    }
}

static void readLine(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    if (!fgets(buffer, (int)size, stdin)) {
        buffer[0] = '\0';
        return;
    }
    trimLine(buffer);
}

/**
 * @brief Entry point, shows and process main menu.
 */
int main(void) {
    int nChoice = 0;

    do {
        nChoice = ShowMainMenu();
        switch (nChoice) {
            case 1:
                ShowProductsMenu();
                break;
            case 2:
                ShowOrdersMenu();
                break;
            case 3:
                ShowCustomersMenu();
                break;
            case 4:
                printf("Closing...\n\n");
                break;
        }
    } while (nChoice != 4);

    return 0;
}


/**
 * @brief prints main menu and allows to select an option.
 */
int ShowMainMenu(void) {
    char command[32];

    for (;;) {
        printf(" (1) use\n"
               " (2) insert\n"
               " (3) print\n"
               " (4) exit\n\n"
               "Enter a number or a command > ");

        if (!fgets(command, (int)sizeof(command), stdin)) {
            return 4;
        }
        trimLine(command);

        if (strcmp(command, "1") == 0 || strcmp(command, "use") == 0) return 1;
        if (strcmp(command, "2") == 0 || strcmp(command, "insert") == 0) return 2;
        if (strcmp(command, "3") == 0 || strcmp(command, "print") == 0) return 3;
        if (strcmp(command, "4") == 0 || strcmp(command, "exit") == 0) return 4;

        printf("Invalid choice. Please try again\n\n");
    }
}


/**
 * @brief Shows and processes the products menu.
 */
void ShowProductsMenu(void) {
    char tableName[MENU_NAME_LENGTH];

    readLine("Table name: ", tableName, sizeof(tableName));
    if (tableName[0] == '\0') {
        printf("Table name cannot be empty\n");
        return;
    }

    if (!createTable(tableName)) {
        printf("Error: could not create or open table %s\n", tableName);
        return;
    }

    strcpy(selectedTable, tableName);
    replaceExtensionByIdx(selectedTable, selectedIndex);
    tableSelected = true;
    printf("Using table %s\n", selectedTable);
}


/**
 * @brief prints products submenu and allows to select an option.
 */
int ShowProductsSubMenu(void) {
    return 0;
}


/**
 * @brief Shows and processes the orders menu.
 */
void ShowOrdersMenu(void) {
    char key[BufferLength];
    char title[BufferLength];
    Book book;

    if (!tableSelected) {
        printf("Error: no table selected. Use 'use' first.\n");
        return;
    }

    readLine("Key (4 characters): ", key, sizeof(key));
    if (strlen(key) != PK_SIZE) {
        printf("Error: the key must contain exactly 4 characters\n");
        return;
    }

    readLine("Title: ", title, sizeof(title));
    memcpy(book.book_id, key, PK_SIZE);
    book.title_len = strlen(title);
    book.title = title;

    if (!addTableEntry(&book, selectedTable, selectedIndex)) {
        printf("Error: the key already exists or the record could not be added\n");
        return;
    }

    printf("Record added\n");
}


/**
 * @brief prints the orders submenu and allows to select an option.
 */
int ShowOrdersSubMenu(void) {
    return 0;
}


/**
 * @brief Shows and processes the customers menu.
 */
void ShowCustomersMenu(void) {
    char levelText[32];
    int level;

    if (!tableSelected) {
        printf("Error: no table selected. Use 'use' first.\n");
        return;
    }

    readLine("Print level (0 for root only): ", levelText, sizeof(levelText));
    level = atoi(levelText);
    if (level < 0) {
        printf("Error: the level cannot be negative\n");
        return;
    }

    printTree((size_t)level, selectedIndex);
}


/**
 * @brief prints the customers submenu and allows to select an option.
 */
int ShowCustomersSubMenu(void) {
    return 0;
}