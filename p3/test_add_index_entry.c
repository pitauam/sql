#include "tester.h"

void checkAddIndexEntry(const char *indexName);

int main(void) {
    const char *indexName = "test_add_index_entry.idx";

    remove(indexName);
    checkAddIndexEntry(indexName);
    remove(indexName);

    return 0;
}
