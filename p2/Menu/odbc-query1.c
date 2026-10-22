#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"
#include <stdio.h> 

/*
 * example 3 with a queries build on-the-fly, the bad way
 */

int main(void) {
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    int ret; /* odbc.c */
    SQLRETURN ret2; /* ODBC API return status */
    #define BufferLength 512
    char x[BufferLength] = "\0";
    char y[BufferLength] = "\0";
    char z[BufferLength] = "\0";
    char b[BufferLength] = "\0";

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

    printf("               customernumber | customername | sum");

    printf("x = ");
    (void) fflush(stdout);
    while (fgets(x, (int) sizeof(x), stdin) != NULL) {
        char query[BufferLength + 28];
        /* snprintf is not defined if ansi flag is enabled */
        /*primer ojo*/
        (void) snprintf(query, (size_t)(BufferLength + 27), "SELECT c.customernumber, c.customername, Sum(py.amount) FROM   payments py JOIN customers c ON py.customernumber = c.customernumber JOIN orders o ON c.customernumber = o.customernumber JOIN orderdetails od ON o.ordernumber = od.ordernumber JOIN products p ON od.productcode = p.productcode WHERE  p.productname = '1940 Ford Pickup Truck' GROUP  BY c.customernumber ORDER  BY Sum(py.amount) DESC;");
        /*copiado de la pizarra, lo ha pedido la profe*/
        printf("%s\n",query);

        (void) SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);

        printf("customernumber | customername | sum");
        /*aqui se asigna a y el resultado de la query (la columna)*/
        (void) SQLBindCol(stmt, 1, SQL_C_CHAR, (SQLCHAR*) y, BufferLength , NULL);
        (void) SQLBindCol(stmt, 2, SQL_C_CHAR, (SQLCHAR*) z, BufferLength , NULL);
        (void) SQLBindCol(stmt, 3, SQL_C_CHAR, (SQLCHAR*) b, BufferLength , NULL);

        /* Loop through the rows in the result-set */
        while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
            printf("%15s %30s %15s\n", y,z,b);
        }

        /*segundo ojo, primero error de sintaxis y segundo no funciona*/
        
        ret2 = SQLCloseCursor(stmt);
        if (!SQL_SUCCEEDED(ret2)) {
            odbc_extract_error("", stmt, SQL_HANDLE_STMT);
           return ret;
        }

        printf("x = ");
        /*limpiar*/
        ret = fflush(stdout);
    }
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

