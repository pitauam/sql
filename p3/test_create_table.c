#include "tester.h"

void checkCreateTable(const char *tableName);

int main(void) {
    const char *tableName = "test_table.dat";

    remove(tableName);
    checkCreateTable(tableName);
    remove(tableName);

    return 0;
}
