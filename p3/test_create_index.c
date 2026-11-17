#include "tester.h"

void checkCreateIndex(const char *indexName);

int main(void) {
    remove("test.idx");
    checkCreateIndex("test.idx");
    return 0;
}