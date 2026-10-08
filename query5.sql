/*Pa´ıses que tienen al menos una oficina que no ha vendido nada durante el a˜no
2003. La salida debe mostrar dos columnas conteniendo el nombre del pa´ıs y el
n´umero de oficinas que no han realizado ninguna venta. Ordena las salidas por
el n´umero de oficinas de forma que la primera l´ınea muestre el pa´ıs con m´as
oficinas que no han realizado ninguna venta.*/

SELECT oc.country           AS "Pais",
       Count(oc.officecode) AS "oficinas con 0 ventas"
FROM   offices oc
WHERE  oc.officecode NOT IN (
                            /*Obtenemos una lista los que si vendieron en 2003 para luego seleccionar los que no esten en la lista(NOT IN)*/
                            SELECT DISTINCT em.officecode
                             FROM   employees em
                                    JOIN customers c
                                      ON em.employeenumber =
                                         c.salesrepemployeenumber
                                    JOIN orders o
                                      ON c.customernumber = o.customernumber
                             WHERE  Extract(year FROM o.orderdate) = 2003)
GROUP  BY oc.country
ORDER  BY Count(oc.officecode) DESC 

