CC = gcc -g
CFLAGS = -Wall -Wextra -pedantic -std=c99
LDLIBS = -lodbc

export PGDATABASE:=classicmodels
export PGUSER:=alumnodb
export PGPASSWORD:=alumnodb
export PGCLIENTENCODING:=LATIN9
export PGHOST:=localhost

DBNAME =$(PGDATABASE)
PSQL = psql
CREATEDB = createdb
DROPDB = dropdb --if-exists
PG_DUMP = pg_dump
PG_RESTORE = pg_restore

EXE = menu

all: classicmodels $(EXE)

compile: $(EXE)

classicmodels: restore

createdb: dropdb
	@echo Creando BBDD
	@$(CREATEDB)

dropdb:
	@echo Eliminando BBDD
	@$(DROPDB) $(DBNAME)

restore: createdb
	@echo restore data base
	@cat $(DBNAME) | $(PSQL)

menu: menu.o odbc.o customers.o orders.o products.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

menu.o: menu.c odbc.h customers.h orders.h products.h
	$(CC) $(CFLAGS) -c $<

customers.o: customers.c customers.h odbc.h
	$(CC) $(CFLAGS) -c $<

orders.o: orders.c orders.h odbc.h
	$(CC) $(CFLAGS) -c $<

products.o: products.c products.h odbc.h
	$(CC) $(CFLAGS) -c $<

odbc.o: odbc.c odbc.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o core $(EXE)

#para correr todos los scripts de pruebas automaticamente: usar pruebas
tests: menu
	@set -e; for script in *.sh; do \
		echo "===== $$script ====="; \
		expect "./$$script"; \
	done
