#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"
#include "customers.h"

#define BufferLength 512


int PrintFindCustomers(void) {
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    int ret; /* odbc.c */
    SQLRETURN ret2; /* ODBC API return status */
    /*nombre del customer a buscar*/
    char customername[BufferLength];
    char contactfirstname[BufferLength];
    char contactlastname[BufferLength];
    int customernumber;
 

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

    /* Pide el customername con scanf como char */
    printf("Enter customer name > ");
    (void) fflush(stdout);
    (void) scanf("%s", customername);
    
    /* Preparamos la consulta con para encontrar el cliente */
    ret = SQLPrepare(stmt, (SQLCHAR*) 
        "SELECT customers.customername, customers.contactfirstname, customers.contactlastname, customers.customernumber FROM customers WHERE customers.contactfirstname LIKE '%%' || ? || '%%' OR customers.contactlastname LIKE '%%' || ? || '%%' ORDER BY customers.customernumber", SQL_NTS);
        
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return EXIT_FAILURE;
    }

    /* Vinculamos la ? con la variable entera usando SQL_C_SLONG y SQL_INTEGER */
    SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &customername, 0, NULL);
    /* Se vuelve a vincular para la segunda ? */
    SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &customername, 0, NULL);

    /* Ejecutamos */
    ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return EXIT_FAILURE;
    }

    /* Asignamos la priemra columna seleccionada al id */
    (void) SQLBindCol(stmt, 1, SQL_C_DOUBLE, &customernumber, 0, NULL);
    /* Asignamos la segunda columna seleccionada al nombre */
    (void) SQLBindCol(stmt, 2, SQL_C_DOUBLE, &contactfirstname, 0, NULL);
    /* Asignamos la tercera columna seleccionada al apellido */
    (void) SQLBindCol(stmt, 3, SQL_C_DOUBLE, &contactlastname, 0, NULL);
    
    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        /* Imprime el saldo exactamente con el formato del expect */
        printf("%i, %s, %s", customernumber, contactfirstname, contactlastname);
    }

    ret2 = SQLCloseCursor(stmt);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }

    /* Limpiar */
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

int PrintListProducts(void) {

    return EXIT_SUCCESS;
}
    

int PrintBalance(){
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    int ret; /* odbc.c */
    SQLRETURN ret2; /* ODBC API return status */
    int customernumber = 0;
    double balance = 0.0;

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

    /* Pide el customernumber con scanf como entero */
    printf("Enter customer number > ");
    (void) fflush(stdout);
    (void) scanf("%d", &customernumber);
    
    /* Preparamos la consulta con subconsultas para calcular pagos menos compras */
    ret = SQLPrepare(stmt, (SQLCHAR*) 
        "SELECT "
        "COALESCE((SELECT SUM(amount) FROM payments WHERE customernumber = c.customernumber), 0) - "
        "COALESCE((SELECT SUM(od.quantityordered * od.priceeach) FROM orders o JOIN orderdetails od ON o.ordernumber = od.ordernumber WHERE o.customernumber = c.customernumber), 0) "
        "FROM customers c "
        "WHERE c.customernumber = ?;", SQL_NTS);
        
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return EXIT_FAILURE;
    }

    /* Vinculamos la ? con la variable entera usando SQL_C_SLONG y SQL_INTEGER */
    SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &customernumber, 0, NULL);

    /* Ejecutamos */
    ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return EXIT_FAILURE;
    }

    /* Asignamos la columna seleccionada al balance */
    (void) SQLBindCol(stmt, 1, SQL_C_DOUBLE, &balance, 0, NULL);
    
    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        /* Imprime el saldo exactamente con el formato del expect */
        printf("Balance = %.2f\n", balance);
    }

    ret2 = SQLCloseCursor(stmt);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }

    /* Limpiar */
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