#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"

#define BufferLength 512

/**
 * @brief prints a list of the orders between two dates
 * 
 * @author Mario Rodriguez
 */
int PrintRange();

/**
 * @brief prints the orders that haven't been shipped
 * 
 * @author Mario Rodriguez
 */
int PrintOpen();

/**
 * @brief prints the informarion of an order(date, status, protucts...)
 * 
 * @author Mario Rodriguez
 */
int PrintDetail();
