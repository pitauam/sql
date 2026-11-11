CC = gcc -g
CFLAGS = -Wall -Wextra -pedantic -std=c99
LDLIBS = -lodbc

EXE = menu

all: $(EXE)

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