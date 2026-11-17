#include "tester.h"

void checkPrint(const char *indexName);

int main(void) {
    const char *indexName = "test_print.idx";

    remove(indexName);
    remove("file.txt");
    checkPrint(indexName);
    remove(indexName);
    remove("file.txt");

    return 0;
}
