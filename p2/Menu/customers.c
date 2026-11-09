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
    SQLLEN name_length = SQL_NTS;
    /*nombre del customer a buscar*/
    char search[BufferLength];
    char customername[BufferLength];
    char contactfirstname[BufferLength];
    char contactlastname[BufferLength];
    SQLINTEGER customernumber;
 
    /*muestra el prompt antes de conectar*/
    printf("Enter customer name > ");
    (void)fflush(stdout);
    if (fgets(search, sizeof(search), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    search[strcspn(search, "\r\n")] = '\0';

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

    /* Preparamos la consulta para encontrar el cliente */
    ret = SQLPrepare(stmt, (SQLCHAR*) 
        "SELECT customers.customername, customers.contactfirstname, customers.contactlastname, customers.customernumber FROM customers WHERE customers.contactfirstname LIKE '%' || ? || '%' OR customers.contactlastname LIKE '%' || ? || '%' ORDER BY customers.customernumber", SQL_NTS);
        
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return EXIT_FAILURE;
    }

    /* Vinculamos el texto buscado a las dos condiciones */
    ret = SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, sizeof(search) - 1, 0, search, sizeof(search), &name_length);
    if (SQL_SUCCEEDED(ret)) {
        ret = SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, sizeof(search) - 1, 0, search, sizeof(search), &name_length);
    }

    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return EXIT_FAILURE;
    }

    /* Ejecutamos */
    ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return EXIT_FAILURE;
    }

    /* Asignamos cada columna al tipo de dato correspondiente */
    (void) SQLBindCol(stmt, 1, SQL_C_CHAR, customername, sizeof(customername), NULL);
    (void) SQLBindCol(stmt, 2, SQL_C_CHAR, contactfirstname, sizeof(contactfirstname), NULL);
    (void) SQLBindCol(stmt, 3, SQL_C_CHAR, contactlastname, sizeof(contactlastname), NULL);
    (void) SQLBindCol(stmt, 4, SQL_C_SLONG, &customernumber, 0, NULL);
    
    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("%ld %s %s %s\n", (long)customernumber, customername, contactfirstname, contactlastname);
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

    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    int ret; /* odbc.c */
    SQLRETURN ret2; /* ODBC API return status */
    SQLLEN name_length = SQL_NTS;
    /*numero del customer a buscar*/
    SQLINTEGER customernumber;
    char productos[BufferLength];
    int numerototalsolicitado;
 
    /*muestra el prompt antes de conectar*/
    printf("Enter customer number > ");
    (void)fflush(stdout);
    if (fgets((int)customernumber, 0, stdin) == NULL) {
        return EXIT_FAILURE;
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

    /* Preparamos la consulta para encontrar el cliente */
    ret = SQLPrepare(stmt, (SQLCHAR*) 
        "SELECT products.productname, SUM(orderdetails.quantityordered) AS 'unidades_totales' FROM orders JOIN orderdetails ON orderdetails.ordernumber = orders.ordernumber JOIN products ON products.productcode = orderdetails.productcode WHERE customers.customernumber = ? GROUP BY products.productcode, products.productname ORDER BY products.productcode ", SQL_NTS);
        
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return EXIT_FAILURE;
    }

    /* Vinculamos el texto buscado a las dos condiciones */
    ret = SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                           sizeof(customernumber) - 1, 0, customernumber, sizeof(customernumber), &name_length);

    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return EXIT_FAILURE;
    }

    /* Ejecutamos */
    ret = SQLExecute(stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return EXIT_FAILURE;
    }

    /* Asignamos cada columna al tipo de dato correspondiente */
    (void) SQLBindCol(stmt, 1, SQL_C_CHAR, productos, sizeof(productos), NULL);
    (void) SQLBindCol(stmt, 2, SQL_INTEGER, numerototalsolicitado, sizeof(numerototalsolicitado), NULL);

    
    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("%s %ld\n", productos, numerototalsolicitado);
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
