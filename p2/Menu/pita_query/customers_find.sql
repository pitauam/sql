/*Find: Se solicita al usuario el nombre de contacto del cliente y se de-
vuelve un listado con todos los clientes cuyo nombre (contactfirstname)
o apellido (contactlastname) de contacto contenga la cadena suministra-
da. El listado debe incluir el nombre del cliente (customername) y el nombre
(contactfirstname) y el apellido (contactlastname) de contacto asi como el
identificador (customernumber) del cliente. Ordena la salida por customernumber.*/

SELECT customers.customername, customers.contactfirstname, customers.contactlastname, customers.customernumber FROM customers WHERE customers.contactfirstname LIKE '%' || ? || '%' OR customers.contactlastname LIKE '%' || ? || '%' ORDER BY customers.customernumber