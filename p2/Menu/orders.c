#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"

#include "orders.h"

int PrintOpen(){
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

int PrintRange(){
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    int ret; /* odbc.c */
    SQLRETURN ret2; /* ODBC API return status */
    SQLLEN ind1 = SQL_NTS;
    SQLLEN ind2 = SQL_NTS;
    char date1[BufferLength] = "\0";
    char date2[BufferLength] = "\0";
    char input_line[BufferLength] = "\0";
    char ordernumber[BufferLength] = "\0";
    char orderdate[BufferLength] = "\0";
    char shippedate[BufferLength] = "\0";

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

    /* Pedir fechas en el formato requerido por el expect */
    printf("Enter dates (YYYY-MM-DD - YYYY-MM-DD) > ");
    (void) fflush(stdout);
    if (fgets(input_line, (int) sizeof(input_line), stdin) != NULL) {
        input_line[strcspn(input_line, "\r\n")] = 0; /* Limpiar el \n */ 
        /* Separar la entrada en date1 y date2 utilizando el guión como separador */
        sscanf(input_line, "%s - %s", date1, date2);
    }
    
    char query[] = "SELECT ordernumber, orderdate, shippeddate FROM orders WHERE orderdate >= ? AND orderdate <= ? ORDER BY ordernumber";
    
    /* preparamos la consulta */
    ret = SQLPrepare(stmt, (SQLCHAR*) query, SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return EXIT_FAILURE;
    }

    /* vinculamos las ? con las variables */
    SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 10, 0, date1, sizeof(date1), &ind1);
    SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 10, 0, date2, sizeof(date2), &ind2);

    /* Ejecutamos */
    ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return EXIT_FAILURE;
    }

    /* Asignar las columnas seleccionadas en el select */
    (void) SQLBindCol(stmt, 1, SQL_C_CHAR, (SQLCHAR*) ordernumber, BufferLength, NULL);
    (void) SQLBindCol(stmt, 2, SQL_C_CHAR, (SQLCHAR*) orderdate, BufferLength, NULL);
    (void) SQLBindCol(stmt, 3, SQL_C_CHAR, (SQLCHAR*) shippedate, BufferLength, NULL);

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        /* Imprimir cada row sin cabeceras y con formato plano para el expect */
        printf("%s %s %s\n", ordernumber, orderdate, shippedate);
    }
        
    ret2 = SQLCloseCursor(stmt);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }

    /* limpiar */
    (void) fflush(stdout);

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

int PrintDetail(){
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    int ret; /* odbc.c */
    SQLRETURN ret2; /* ODBC API return status */
    SQLLEN ind1 = SQL_NTS;
    char ordernumber[BufferLength] = "\0";
    char orderdate[BufferLength] = "\0";
    char productcode[BufferLength] = "\0";
    char status[BufferLength] = "\0";
    int quantity;
    double price;


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
    /*pide x*/


    printf("Enter ordernumber > ");
    (void) fflush(stdout);
    if (fgets(ordernumber, (int) sizeof(ordernumber), stdin) != NULL) {
        ordernumber[strcspn(ordernumber, "\r\n")] = 0; /*Limpiar el \n*/ 
    }
    
    
    /* preparamos la consulta */
    ret = SQLPrepare(stmt, (SQLCHAR*) "SELECT o.orderdate, o.status, od.quantityordered, od.priceeach, od.productcode FROM orders o JOIN orderdetails od ON o.ordernumber = od.ordernumber WHERE od.ordernumber = ? ORDER BY od.orderlinenumber; ", SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return EXIT_FAILURE;
    }

    /* vinculamos las ? con las variables */
    SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 10, 0, ordernumber, sizeof(ordernumber), &ind1);

    /* Ejecutamos */
    ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return EXIT_FAILURE;
    }
        /*aqui se asigna a y la columna seleccionada en el select*/
        (void) SQLBindCol(stmt, 1, SQL_C_CHAR, (SQLCHAR*) orderdate, BufferLength , NULL);
        (void) SQLBindCol(stmt, 2, SQL_C_CHAR, (SQLCHAR*) status, BufferLength , NULL);
        (void) SQLBindCol(stmt, 3, SQL_C_SLONG, &quantity, 0 , NULL);
        (void) SQLBindCol(stmt, 4, SQL_C_DOUBLE, &price, 0 , NULL);
        (void) SQLBindCol(stmt, 5, SQL_C_CHAR, (SQLCHAR*) productcode, BufferLength , NULL);
        
        /* Loop through the rows in the result-set */
        while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
            /*imprime cada row*/
            printf("%15s %d %.2f\n", productcode, quantity, price);

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

