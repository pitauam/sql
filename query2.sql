SELECT p.productline                                          AS
       "tipo de producto",
       Round(Avg(shippeddate :: DATE - orderdate :: DATE), 2) AS
       "tiempo de entrega medio"
FROM   orders o
       join orderdetails od
         ON o.ordernumber = od.ordernumber
       join products p
         ON od.productcode = p.productcode
       join productlines pl
         ON pl.productline = p.productline
GROUP  BY p.productline;