#include "tester.h"

void checkFindKey(char *indexName);

int main(void) {
    char indexName[] = "test_find_key.idx";

    remove(indexName);
    checkFindKey(indexName);
    remove(indexName);

    return 0;
}
