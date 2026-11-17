#include "tester.h"

void checkReplaceExtensionByIdx(const char *tableName, const char *indexName);

int main(void) {
    checkReplaceExtensionByIdx("myDataBase.dat", "myDataBase.idx");
    return 0;
}
