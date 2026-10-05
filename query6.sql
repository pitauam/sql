/*6. Definimos el carro de la compra como el conjunto de 
 todos los productos comprados usando la misma “order”. 
 Se desea un listado de todas las parejas de
productos que aparezcan en m´as de un carro de la compra. 
La salida debe mostrar tres l´ıneas conteniendo el identificador
de ambos productos y el numero*/


SELECT o1.productcode AS producto_1,
       o2.productcode AS producto_2,
       Count(*)
FROM   orderdetails o1
       JOIN orderdetails o2
         ON o1.ordernumber = o2.ordernumber
            AND o1.productcode != o2.productcode
GROUP  BY o1.productcode,
          o2.productcode
HAVING Count(*) > 1
ORDER  BY Count(*) DESC
LIMIT  3; 


