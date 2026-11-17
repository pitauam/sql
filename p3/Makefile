CC = gcc -g
CFLAGS = -Wall -Wextra -pedantic -ansi
# LDLIBS = -lodbc

# recompile if this header changes
HEADERS = utils.h

EXE = tester
OBJ = $(EXE).o utils.o checkUtils.o checkCreateTable.o checkReplaceExtensionByIdx.o \
checkCreateIndex.o checkPrint.o checkFindKey.o checkAddIndexEntry.o
#all : $(EXE)

.PHONY: clean tester_valgrind test_create_index test_create_table

%.o: %.c $(HEADERS)
	@echo Compiling $<...
	$(CC) $(CFLAGS) -c -o $@ $<

#estaba mal el makefile descargado sin tocar
$(EXE): $(DEPS) $(OBJ)
	$(CC) -o $(EXE) $(OBJ) $(LDLIBS)

clean :
	rm -f *.o core $(EXE) test_create_index test_create_table

tester_valgrind: $(EXE)
	valgrind ./$(EXE)

test_create_index:
	gcc -g -Wall -Wextra -pedantic -ansi \
	-o test_create_index test_create_index.c checkCreateIndex.c utils.c
	./test_create_index

test_create_table:
	$(CC) $(CFLAGS) -o test_create_table \
		test_create_table.c checkCreateTable.c utils.c
	./test_create_table
