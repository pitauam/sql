-- http://dpriver.com (Cumpliendo el requisito de limpieza)

SELECT 
    c.customernumber, 
    c.customername, 
    SUM(p.amount) AS total_pagado
FROM 
    customers c
-- 1. Unimos con pagos para calcular el dinero total abonado
JOIN 
    payments p ON c.customernumber = p.customernumber
WHERE 
    c.customernumber IN (
        -- 2. Subconsulta: Buscamos qué clientes compraron ese camión específico
        SELECT DISTINCT 
            o.customernumber
        FROM 
            orders o
        JOIN 
            orderdetails od ON o.ordernumber = od.ordernumber
        JOIN 
            products pr ON od.productcode = pr.productcode
        WHERE 
            pr.productname = '1940 Ford Pickup Truck'
    )
-- 3. Agrupamos por cliente para poder hacer la suma (SUM)
GROUP BY 
    c.customernumber, 
    c.customername
-- 4. Ordenamos de mayor a menor cantidad abonada
ORDER BY 
    total_pagado DESC;
