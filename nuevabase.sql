/*4. Redise ̃no de la Base de Datos
El dise ̃no de la base de datos es en general bastante pobre y muestra m ́ultiples
deficiencias. Por ejemplo: si un empleado se mueve de una oficina a otra se pierde
la informaci ́on de la/s oficina/s donde hab ́ıa trabajado en el pasado, un cliente solo
puede relacionarse con un  ́unico empleado o los pagos no est ́an asociados a una
compra en concreto.
Dise ̃na una base de datos que evite los inconvenientes citados. Incluye en la
memoria: (1) el nuevo diagrama relacional y comenta c ́omo tus cambios solucionan
los problemas planteados y (2) a ̃nade un nuevo comando en el fichero makefile que
se ejecute con makefile nuevabase y que borre todas las tablas y datos de la base
de datos y cree las tablas de tu nuevo dise ̃no (los comandos SQL necesarios para
crear las tablas se almacenar ́an en un fichero llamado nuevabase.sql).
*/
/*
empleado se mueve de oficina => se pierde info de oficinas donde ha trabajado en el pasado
un cliente no puede relacionarse con mas de un empleado
los pagos no estan asociados a una compra
*/

DROP TABLE IF EXISTS public.historial_oficinas CASCADE;
DROP TABLE IF EXISTS public.cliente_empleado CASCADE;
DROP TABLE IF EXISTS public.payments CASCADE;
DROP TABLE IF EXISTS public.orderdetails CASCADE;
DROP TABLE IF EXISTS public.orders CASCADE;
DROP TABLE IF EXISTS public.customers CASCADE;
DROP TABLE IF EXISTS public.employees CASCADE;
DROP TABLE IF EXISTS public.offices CASCADE;
DROP TABLE IF EXISTS public.productlines CASCADE;
DROP TABLE IF EXISTS public.products CASCADE;
DROP TABLE IF EXISTS public.employeehistory CASCADE; --primera tabla para resolver el primer problema
DROP TABLE IF EXISTS public.customerrep CASCADE; --primera tabla para resolver el primer problema


CREATE TABLE public.customerrep (
    customernumber integer NOT NULL, --references customernumber
    salesrepemployeenumber integer NOT NULL --references employeenumber
    --primary key customernumber y salesrep
);

ALTER TABLE public.customerrep OWNER TO alumnodb;


CREATE TABLE public.customers (
    customernumber integer NOT NULL,
    customername character varying(50) NOT NULL,
    contactlastname character varying(50) NOT NULL,
    contactfirstname character varying(50) NOT NULL,
    phone character varying(50) NOT NULL,
    addressline1 character varying(50) NOT NULL,
    addressline2 character varying(50) DEFAULT NULL::character varying,
    city character varying(50) NOT NULL,
    state character varying(50) DEFAULT NULL::character varying,
    postalcode character varying(15) DEFAULT NULL::character varying,
    country character varying(50) NOT NULL,
    --salesrepemployeenumber integer,
    creditlimit numeric(10,2) DEFAULT NULL::numeric
);


ALTER TABLE public.customers OWNER TO alumnodb;

--
-- Name: employees; Type: TABLE; Schema: public; Owner: alumnodb
--

CREATE TABLE public.employees (
    employeenumber integer NOT NULL,
    lastname character varying(50) NOT NULL,
    firstname character varying(50) NOT NULL,
    extension character varying(10) NOT NULL,
    email character varying(100) NOT NULL,
    --officecode character varying(10) NOT NULL,
    reportsto integer,
    jobtitle character varying(50) NOT NULL
);


ALTER TABLE public.employees OWNER TO alumnodb;

--
-- Name: offices; Type: TABLE; Schema: public; Owner: alumnodb
--

CREATE TABLE public.offices (
    officecode character varying(10) NOT NULL,
    city character varying(50) NOT NULL,
    phone character varying(50) NOT NULL,
    addressline1 character varying(50) NOT NULL,
    addressline2 character varying(50) DEFAULT NULL::character varying,
    state character varying(50) DEFAULT NULL::character varying,
    country character varying(50) NOT NULL,
    postalcode character varying(15) NOT NULL,
    territory character varying(10) NOT NULL
);


ALTER TABLE public.offices OWNER TO alumnodb;

--
-- Name: orderdetails; Type: TABLE; Schema: public; Owner: alumnodb
--

CREATE TABLE public.orderdetails (
    ordernumber integer NOT NULL,
    productcode character varying(15) NOT NULL,
    quantityordered integer NOT NULL,
    priceeach numeric(10,2) NOT NULL,
    orderlinenumber smallint NOT NULL
);


ALTER TABLE public.orderdetails OWNER TO alumnodb;

--
-- Name: orders; Type: TABLE; Schema: public; Owner: alumnodb
--

CREATE TABLE public.orders (
    ordernumber integer NOT NULL,
    orderdate date NOT NULL,
    requireddate date NOT NULL,
    shippeddate date,
    status character varying(15) NOT NULL,
    comments text,
    customernumber integer NOT NULL
);


ALTER TABLE public.orders OWNER TO alumnodb;

--
-- Name: payments; Type: TABLE; Schema: public; Owner: alumnodb
--

CREATE TABLE public.payments (
    customernumber integer NOT NULL,
    checknumber character varying(50) NOT NULL,
    paymentdate date NOT NULL,
    amount numeric(10,2) NOT NULL
);


ALTER TABLE public.payments OWNER TO alumnodb;

--
-- Name: productlines; Type: TABLE; Schema: public; Owner: alumnodb
--

CREATE TABLE public.productlines (
    productline character varying(50) NOT NULL,
    textdescription character varying(4000) DEFAULT NULL::character varying,
    htmldescription character varying(4000),
    image character varying
);


ALTER TABLE public.productlines OWNER TO alumnodb;

--
-- Name: products; Type: TABLE; Schema: public; Owner: alumnodb
--

CREATE TABLE public.products (
    productcode character varying(15) NOT NULL,
    productname character varying(70) NOT NULL,
    productline character varying(50) NOT NULL,
    productscale character varying(10) NOT NULL,
    productvendor character varying(50) NOT NULL,
    productdescription text NOT NULL,
    quantityinstock smallint NOT NULL,
    buyprice numeric(10,2) NOT NULL,
    msrp numeric(10,2) NOT NULL
);


ALTER TABLE public.products OWNER TO alumnodb;


CREATE TABLE public.employeehistory (
    employeenumber integer NOT NULL, --numero del empleado
    officecode character varying(10) NOT NULL, --codigo de la oficina
    --working integer NOT NULL -- es 0 si no esta trabajando en esta oficina o 1 si está trabajando en esa oficina
    startdate date NOT NULL, --nunca puede ser null, debe empezar a trabajar en una fecha
    enddate date DEFAULT NULL --por defecto se pone a null indicando que es su oficina actual
);


ALTER TABLE public.employeehistory OWNER TO alumnodb;


ALTER TABLE ONLY public.customers
    ADD CONSTRAINT customers_pkey PRIMARY KEY (customernumber);


--
-- Name: employees employees_pkey; Type: CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.employees
    ADD CONSTRAINT employees_pkey PRIMARY KEY (employeenumber);


--
-- Name: offices offices_pkey; Type: CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.offices
    ADD CONSTRAINT offices_pkey PRIMARY KEY (officecode);


--
-- Name: orderdetails orderdetails_pkey; Type: CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.orderdetails
    ADD CONSTRAINT orderdetails_pkey PRIMARY KEY (ordernumber, productcode);


--
-- Name: orders orders_pkey; Type: CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.orders
    ADD CONSTRAINT orders_pkey PRIMARY KEY (ordernumber);


--
-- Name: payments payments_pkey; Type: CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.payments
    ADD CONSTRAINT payments_pkey PRIMARY KEY (customernumber, checknumber);


--
-- Name: productlines productlines_pkey; Type: CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.productlines
    ADD CONSTRAINT productlines_pkey PRIMARY KEY (productline);


--
-- Name: products products_pkey; Type: CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.products
    ADD CONSTRAINT products_pkey PRIMARY KEY (productcode);



--
-- Name: employeehistory employeehistory_ibfk_2; Type: PK CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.employeehistory
    ADD CONSTRAINT employeehistory_pkey PRIMARY KEY (employeenumber, officecode, startdate); 


--
-- Name: customerrep customerrep_ibfk_1; Type: PK CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.customerrep
    ADD CONSTRAINT customerrep_pkey PRIMARY KEY (customernumber, salesrepemployeenumber); 

    












--ALTER TABLE ONLY public.customers
--  ADD CONSTRAINT customers_ibfk_1 FOREIGN KEY (salesrepemployeenumber) REFERENCES public.employees(employeenumber);


--
-- Name: employees employees_ibfk_1; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.employees
    ADD CONSTRAINT employees_ibfk_1 FOREIGN KEY (reportsto) REFERENCES public.employees(employeenumber);


--Hay que añadir employeehistory como un foreign key
--
-- Name: employeehistory employeehistory_ibfk_1; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.employeehistory
    ADD CONSTRAINT employeehistory_ibfk_1 FOREIGN KEY (officecode) REFERENCES public.offices(officecode);


--
-- Name: employeehistory employeehistory_ibfk_2; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--
ALTER TABLE ONLY public.employeehistory
    ADD CONSTRAINT employeehistory_ibfk_2 FOREIGN KEY (employeenumber) REFERENCES public.employees(employeenumber);



--
-- Name: orderdetails orderdetails_ibfk_1; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.orderdetails
    ADD CONSTRAINT orderdetails_ibfk_1 FOREIGN KEY (ordernumber) REFERENCES public.orders(ordernumber);


--
-- Name: orderdetails orderdetails_ibfk_2; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.orderdetails
    ADD CONSTRAINT orderdetails_ibfk_2 FOREIGN KEY (productcode) REFERENCES public.products(productcode);


--
-- Name: orders orders_ibfk_1; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.orders
    ADD CONSTRAINT orders_ibfk_1 FOREIGN KEY (customernumber) REFERENCES public.customers(customernumber);


--
-- Name: payments payments_ibfk_1; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.payments
    ADD CONSTRAINT payments_ibfk_1 FOREIGN KEY (customernumber) REFERENCES public.customers(customernumber);


--
-- Name: products products_ibfk_1; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.products
    ADD CONSTRAINT products_ibfk_1 FOREIGN KEY (productline) REFERENCES public.productlines(productline);



--
-- Name: customerrep customerrep_ibfk_1; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.customerrep
    ADD CONSTRAINT customerrep_ibfk_1 FOREIGN KEY (customernumber) REFERENCES public.customers(customernumber);


--
-- Name: customerrep customerrep_ibfk_2; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.customerrep
    ADD CONSTRAINT customerrep_ibfk_2 FOREIGN KEY (salesrepemployeenumber) REFERENCES public.employees(employeenumber);

--
-- PostgreSQL database dump complete
--


-- NOTA: falta enlazarlo con el empleado (employeenumber).