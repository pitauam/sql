SELECT c.customernumber, c.customername, py.amount
FROM payments py
    JOIN customers c 
     ON py.customernumber = c.customernumber
    JOIN orders o 
     ON c.customernumber = o.customernumber 
    JOIN orderdetails od 
     ON o.ordernumber = od.ordernumber 
    JOIN products p 
     ON od.productcode = p.productcode 
     WHERE p.productname = '1940 Ford Pickup Truck'
ORDER BY py.amount ASC;
