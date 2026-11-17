CC = gcc -g
CFLAGS = -Wall -Wextra -pedantic -ansi
# LDLIBS = -lodbc

# recompile if this header changes
HEADERS = utils.h

EXE = tester
OBJ = $(EXE).o utils.o checkUtils.o checkCreateTable.o checkReplaceExtensionByIdx.o \
checkCreateIndex.o checkPrint.o checkFindKey.o checkAddIndexEntry.o
#all : $(EXE)


%.o: %.c $(HEADERS)
	@echo Compiling $<...
	$(CC) $(CFLAGS) -c -o $@ $<

#estaba mal el makefile descargado sin tocar
$(EXE): $(DEPS) $(OBJ)
	$(CC) -o $(EXE) $(OBJ) $(LDLIBS)

clean :
	rm -f *.o core $(EXE) \
		test_create_index test_create_table test_replace_extension_by_idx \
		test_print_tree test_find_key test_add_index_entry test_add_table_entry \
		test.idx test_table.dat test_table.idx \
		test_print.idx test_find_key.idx test_add_index_entry.idx \
		test_add_table_entry.dat test_add_table_entry.idx \
		myDataBase.dat myDataBase.idx file.txt

# a partir de aqui son todos tests individuales

tester_valgrind: $(EXE)
	valgrind ./$(EXE)

test_create_index:
	$(CC) $(CFLAGS) -o $@ test_create_index.c checkCreateIndex.c utils.c
	./test_create_index

test_create_table:
	$(CC) $(CFLAGS) -o test_create_table \
		test_create_table.c checkCreateTable.c utils.c
	./test_create_table

test_replace_extension_by_idx:
	$(CC) $(CFLAGS) -o $@ \
		test_replace_extension_by_idx.c checkReplaceExtensionByIdx.c utils.c
	./$@

test_print_tree:
	$(CC) $(CFLAGS) -o $@ \
		test_print_tree.c test_globals.c checkUtils.c checkPrint.c utils.c
	./$@

test_find_key:
	$(CC) $(CFLAGS) -o $@ \
		test_find_key.c test_globals.c checkUtils.c checkFindKey.c utils.c
	./$@

test_add_index_entry:
	$(CC) $(CFLAGS) -o $@ \
		test_add_index_entry.c test_globals.c checkUtils.c \
		checkAddIndexEntry.c utils.c
	./$@

test_add_table_entry:
	$(CC) $(CFLAGS) -o $@ test_add_table_entry.c utils.c
	./$@

alltests:
	make test_create_index
	make test_create_table
	make test_replace_extension_by_idx
	make test_print_tree
	make test_find_key
	make test_add_index_entry
	make test_add_table_entry
