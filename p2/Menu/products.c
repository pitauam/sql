#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"
#include "products.h"

#define BufferLength 512




/**
 * @brief prints Stock of Products
 *
 * @return void
 * @author Santiago Pita
 *
 */
int PrintStock() {
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    int ret; 
    SQLRETURN ret2; 
    char x[BufferLength] = "\0"; /* Para guardar el productcode (input del usuario) */
    char y[BufferLength] = "\0"; /* Para guardar el quantityinstock (resultado) */
    char query[BufferLength + 100];

    /*Pide el productcode antes de conectar para que expect pueda responder*/
    printf("Enter productcode > ");
    (void) fflush(stdout);
    if (fgets(x, (int) sizeof(x), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    x[strcspn(x, "\r\n")] = 0;

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        printf("Error al conectar a la BD\n");
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", dbc, SQL_HANDLE_DBC);
        odbc_disconnect(env, dbc);
        return EXIT_FAILURE;
    }

    (void) snprintf(query, sizeof(query), "SELECT quantityinstock FROM products WHERE productcode = '%s';", x);

    ret = SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        (void)SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        (void)odbc_disconnect(env, dbc);
        return EXIT_FAILURE;
    }

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

    return EXIT_SUCCESS;
}

int PrintFindProducts(void){


    return EXIT_SUCCESS;
}