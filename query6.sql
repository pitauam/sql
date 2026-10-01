/*6. Definimos el carro de la compra como el conjunto de 
 todos los productos comprados usando la misma “order”. 
 Se desea un listado de todas las parejas de
productos que aparezcan en m´as de un carro de la compra. 
La salida debe mostrar tres l´ıneas conteniendo el identificador
de ambos productos y el numero*/


SELECT productcode AS producto_1, productcode AS producto_2,
COUNT(proucto_1 AND producto_2)
FROM orderdetails o
JOIN o.