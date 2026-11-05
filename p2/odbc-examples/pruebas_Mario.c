#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"


#define BufferLength 512

int main(void){
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    int ret; /* odbc.c */
    SQLRETURN ret2; /* ODBC API return status */
    char y[BufferLength] = "\0";

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return ret;
    }

    printf("ordernumber");

    (void) fflush(stdout);
    /*comprueba que x sea algo para poder ejecutar la query*/ 
    char query[BufferLength + 28];
        /* snprintf is not defined if ansi flag is enabled */

        /*primer ojo*/
    (void) snprintf(query, (size_t)(BufferLength + 27), "SELECT o.ordernumber FROM orders o WHERE o.shippeddate IS NULL ORDER BY o.ordernumber");
        /*copiado de la pizarra, lo ha pedido la profe*/
    printf("%s\n",query);

    (void) SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);

        /*aqui se asigna a y la columna seleccionada en el select*/
    (void) SQLBindCol(stmt, 1, SQL_C_CHAR, (SQLCHAR*) y, BufferLength , NULL);

        /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        /*imprime cada row*/
        printf("%15s\n", y);
    }

        /*segundo ojo, primero error de sintaxis y segundo no funciona*/
        
    ret2 = SQLCloseCursor(stmt);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }

        /*limpiar*/
    ret = fflush(stdout);
    
    printf("\n");
    
    /* free up statement handle */
    ret2 = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}