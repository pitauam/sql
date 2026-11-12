#ifndef CUSTOMERS_H
#define CUSTOMERS_H

/**
 * @brief asks the user for the name of a contact of a client and returns a list with all the clients whose name or last name contains the user's input, ordered by customernumber
 * 
 * @author Santiago Pita
 */
int PrintFindCustomers(void);

/**
 * @brief asks the user for an id of a client and returns a list with all the products ordered the client in any order. It shows the product name and the total of units the client ordered.
 * 
 * @author Santiago Pita
 */
int PrintListProducts(void);

/**
 * @brief prints the balance of a customer
 * 
 * @author Mario Rodriguez
 */
int PrintBalance(void);

#endif