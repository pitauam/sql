/*
#3. Empleados que reportan a otros empleados que reportan al director.
El director es aquella persona que no reporta a nadie. El listado debe mostrar el
“employeenumber” y el “lastname”.


#primer SELECT: encontrar al director

SELECT e.employeenumber, e.lastname
FROM employees e
WHERE e.reportsto IS NULL;

#segundo SELECT: empleados que reportan a alguien

SELECT e.employeenumber, e.lastname FROM employees e
WHERE e.reportsto IS NOT NULL;

#tercer SELECT: empleados que reportan a personas que reportan al director

SELECT e.employeenumber, e.lastname FROM employees e
WHERE e.reportsto IN
	(
	SELECT jefe.employeenumber FROM employees jefe
	WHERE jefe.reportsto = 
	(
		SELECT director.employeenumber
		FROM employees director
		WHERE director.reportsto IS NULL
		)
	);
	*/


SELECT e.employeenumber,
       e.lastname
FROM   employees e
WHERE  e.reportsto IN (SELECT jefe.employeenumber
                       FROM   employees jefe
                       WHERE  jefe.reportsto = (SELECT director.employeenumber
                                                FROM   employees director
                                                WHERE
                              director.reportsto IS NULL)); 



	

