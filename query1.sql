/*Muestra la cantidad total de dinero abonado por los clientes que han adquirido el “1940 Ford Pickup Truck” (el dinero puede haber sido abonado para
comprar otros modelos). Ordena el resultado por la cantidad de dinero abonada de mayor a menor cantidad. Cada l´ınea debe mostrar: “customernumber”,
“customername” y la cantidad total de dinero pagada.*/


SELECT c.customernumber,
       c.customername,
       Sum(py.amount)
FROM   payments py
       JOIN customers c
         ON py.customernumber = c.customernumber
       JOIN orders o
         ON c.customernumber = o.customernumber
       JOIN orderdetails od
         ON o.ordernumber = od.ordernumber
       JOIN products p
         ON od.productcode = p.productcode
WHERE  p.productname = '1940 Ford Pickup Truck'
GROUP  BY c.customernumber
ORDER  BY Sum(py.amount) DESC;

