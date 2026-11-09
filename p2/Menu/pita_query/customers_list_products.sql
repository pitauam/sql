/*List Products: Se solicita el identificador de un cliente (customernumber) y
se devuelve un listado con todos los productos solicitados por el cliente en
cualquier pedido (“orders”). La salida costar ́a de una linea por cada producto
solicitado mostrando el nombre del producto productname y n ́umero TOTAL
de unidades solicitadas. Esto es, si el mismo producto se ha solicitado en va-
rios pedidos debe aparecer una  ́unica l ́ınea con la suma de todas las unidades
solicitadas. Ordenar la salida por productcode.*/


SELECT products.productname, SUM(orderdetails.quantityordered) FROM orders JOIN orderdetails ON orderdetails.ordernumber = orders.ordernumber JOIN products ON products.productcode = orderdetails.productcode WHERE orders.customernumber = ? GROUP BY products.productcode, products.productname ORDER BY products.productcode