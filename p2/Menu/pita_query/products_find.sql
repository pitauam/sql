/*Find: Se solicita al usuario el nombre del producto y se devuelve un listado
con todos los productos cuyo nombre contenga la cadena suministrada. Cada
línea de la salida contendrá dos cadenas productcode y productname. La sa-
lida estar ́a ordenada por el atributo productcode. N ́otese que la salida puede
constar de m ́as de un producto*/

SELECT productcode, productname FROM products WHERE productname LIKE '%' || ? || '%' ORDER BY productcode;
/*|| concatena*/