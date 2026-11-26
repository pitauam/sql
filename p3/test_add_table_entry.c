#include "tester.h"

void checkAddTableEntry(const char *dataName, const char *indexName);

int main(void) {
    const char *dataName = "test_add_table_entry.dat";
    const char *indexName = "test_add_table_entry.idx";

    checkAddTableEntry(dataName, indexName);
    return 0;
}
