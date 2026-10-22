/*Oficina que ha vendido el mayor n´umero de objetos. Nota: en un pedido (“order”) se puede vender m´as de una unidad de cada producto, cada unidad se
considerar´a un objeto. La salida debe mostrar el “officecode” y el n´umero de
productos vendidos.*/



SELECT oc.officecode AS "Numero de oficina",
       Count(o)      AS "Mayor numero de ordenes"
FROM   offices oc
       JOIN employees em
         ON oc.officecode = em.officecode
       JOIN customers c
         ON em.employeenumber = c.salesrepemployeenumber
       JOIN orders o
         ON c.customernumber = o.customernumber
GROUP  BY oc.officecode
ORDER  BY Count(o) DESC
LIMIT  1; 
