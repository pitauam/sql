SELECT oc.officecode AS 
    "Numero de oficina", 
    COUNT(o) AS 
    "Mayor numero de ordenes"
FROM offices oc
    JOIN employees em 
     ON oc.officecode = em.officecode
    JOIN customers c 
     ON em.employeenumber = c.salesrepemployeenumber
    JOIN orders o 
     ON c.customernumber = o.customernumber
GROUP BY oc.officecode
ORDER BY COUNT(o) DESC
LIMIT 1;
