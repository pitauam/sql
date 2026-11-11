#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"
#include "orders.h"

#define BufferLength 512

int PrintOpen(void) {
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    int ret; /* odbc.c */
    SQLRETURN ret2; /* ODBC API return status */
    char y[BufferLength] = "";

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
    
    /* pedimos el ordernumber */
    printf("ordernumber");
    (void) fflush(stdout);
    
    {
        char query[BufferLength + 28];
        (void) snprintf(query, sizeof(query), "SELECT o.ordernumber FROM orders o WHERE o.shippeddate IS NULL ORDER BY o.ordernumber");
        printf("%s\n", query);
        (void) SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);
    }

    /* aqui se asigna a y la columna seleccionada en el select */
    (void) SQLBindCol(stmt, 1, SQL_C_CHAR,   (SQLCHAR*) y, (SQLLEN)sizeof(y), NULL);

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        /* imprime cada row */
        printf("%15s\n", y);
    }

    ret2 = SQLCloseCursor(stmt);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }

    /* limpiar */
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

int PrintRange(void) {
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    int ret; /* odbc.c */
    SQLRETURN ret2; /* ODBC API return status */
    SQLLEN ind1 = SQL_NTS;
    SQLLEN ind2 = SQL_NTS;
    char date1[BufferLength] = "";
    char date2[BufferLength] = "";
    char input_line[BufferLength];
    char ordernumber[BufferLength] = "";
    char orderdate[BufferLength] = "";
    char shippedate[BufferLength] = "";


    /* Pedir fechas antes de la conexion */
    printf("Enter dates (YYYY-MM-DD - YYYY-MM-DD) > ");
    (void) fflush(stdout);
    if (fgets(input_line, (int) sizeof(input_line), stdin) != NULL) {
        input_line[strcspn(input_line, "\r\n")] = '\0'; /* Limpiar el \n */ 
        /* Separar la entrada en date1 y date2 utilizando el formato con guion */
        (void) sscanf(input_line, "%s - %s", date1, date2);
    }

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

    {
        char query[] = "SELECT ordernumber, orderdate, shippeddate FROM orders WHERE orderdate >= ? AND orderdate <= ? ORDER BY ordernumber";
        
        /* preparamos la consulta */
        ret = SQLPrepare(stmt, (SQLCHAR*) query, SQL_NTS);
        if (!SQL_SUCCEEDED(ret)) {
            odbc_extract_error("", stmt, SQL_HANDLE_STMT);
            return EXIT_FAILURE;
        }
    }

    /* vinculamos las ? con las variables */
    (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 10, 0, date1, (SQLLEN)sizeof(date1), &ind1);
    (void) SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 10, 0, date2, (SQLLEN)sizeof(date2), &ind2);

    /* Ejecutamos */
    ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return EXIT_FAILURE;
    }

    /* Asignar las columnas seleccionadas en el select */
    (void) SQLBindCol(stmt, 1, SQL_C_CHAR,   (SQLCHAR*) ordernumber, (SQLLEN)sizeof(ordernumber), NULL);
    (void) SQLBindCol(stmt, 2, SQL_C_CHAR,   (SQLCHAR*) orderdate, (SQLLEN)sizeof(orderdate), NULL);
    (void) SQLBindCol(stmt, 3, SQL_C_CHAR,   (SQLCHAR*) shippedate, (SQLLEN)sizeof(shippedate), NULL);

    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        /* Imprimir cada row */
        printf("%s %s %s\n", ordernumber, orderdate, shippedate);
    }
        
    ret2 = SQLCloseCursor(stmt);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }

    /* limpiar */
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

int PrintDetail(void) {
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    int ret; /* odbc.c */
    SQLRETURN ret2; /* ODBC API return status */
    SQLLEN ind1 = SQL_NTS;
    char ordernumber[BufferLength] = "";
    char orderdate[BufferLength] = "";
    char productcode[BufferLength] = "";
    char status[BufferLength] = "";
    int quantity = 0;
    double price = 0.0;

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

    printf("Enter ordernumber > ");
    (void) fflush(stdout);
    if (fgets(ordernumber, (int) sizeof(ordernumber), stdin) != NULL) {
        ordernumber[strcspn(ordernumber, "\r\n")] = '\0'; /* Limpiar el \n */ 
    }
    
    /* preparamos la consulta */
    ret = SQLPrepare(stmt, (SQLCHAR*) "SELECT o.orderdate, o.status, od.quantityordered, od.priceeach, od.productcode FROM orders o JOIN orderdetails od ON o.ordernumber = od.ordernumber WHERE od.ordernumber = ? ORDER BY od.orderlinenumber;", SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return EXIT_FAILURE;
    }

    /* vinculamos las ? con las variables */
    (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 10, 0, ordernumber, (SQLLEN)sizeof(ordernumber), &ind1);

    /* Ejecutamos */
    ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return EXIT_FAILURE;
    }

    /* aqui se asigna a las variables la columna seleccionada en el select */
    (void) SQLBindCol(stmt, 1, SQL_C_CHAR,   (SQLCHAR*) orderdate, (SQLLEN)sizeof(orderdate), NULL);
    (void) SQLBindCol(stmt, 2, SQL_C_CHAR,   (SQLCHAR*) status, (SQLLEN)sizeof(status), NULL);
    (void) SQLBindCol(stmt, 3, SQL_C_SLONG,   &quantity, 0, NULL);
    (void) SQLBindCol(stmt, 4, SQL_C_DOUBLE,   &price, 0, NULL);
    (void) SQLBindCol(stmt, 5, SQL_C_CHAR,   (SQLCHAR*) productcode, (SQLLEN)sizeof(productcode), NULL);
    
    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("%15s %d %.2f\n", productcode, quantity, price);
    }

    ret2 = SQLCloseCursor(stmt);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }

    /* limpiar */
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