#include "utils.h"
#include <string.h>

int main(void) {
    const char *dataName = "test_add_table_entry.dat";
    const char *indexName = "test_add_table_entry.idx";
    const char title[] = "Test title";
    Book book;
    FILE *fp;
    int header = NO_DELETED_REGISTERS;
    int titleLength = (int)strlen(title);
    int storedLength = 0;
    int dataOffset = -1;
    char storedTitle[sizeof(title)];
    char key[PK_SIZE] = {'A', 'B', 'C', 'D'};
    int foundOffset = -1;

    remove(dataName);
    remove(indexName);

    fp = fopen(dataName, "wb");
    if (fp == NULL || fwrite(&header, sizeof(header), 1, fp) != 1) {
        fprintf(stderr, "Could not initialize test data file\n");
        if (fp != NULL) fclose(fp);
        return 1;
    }
    fclose(fp);

    fp = fopen(indexName, "wb");
    if (fp == NULL || fwrite(&header, sizeof(header), 1, fp) != 1 ||
        fwrite(&header, sizeof(header), 1, fp) != 1) {
        fprintf(stderr, "Could not initialize test index file\n");
        if (fp != NULL) fclose(fp);
        return 1;
    }
    fclose(fp);

    memcpy(book.book_id, key, PK_SIZE);
    book.title_len = (size_t)titleLength;
    book.title = (char *)title;

    if (!addTableEntry(&book, dataName, indexName)) {
        fprintf(stderr, "addTableEntry returned false\n");
        return 1;
    }

    fp = fopen(dataName, "rb");
    if (fp == NULL || fseek(fp, DATA_HEADER_SIZE, SEEK_SET) != 0 ||
        fread(book.book_id, PK_SIZE, 1, fp) != 1 ||
        fread(&storedLength, sizeof(storedLength), 1, fp) != 1 ||
        storedLength != titleLength ||
        fread(storedTitle, (size_t)storedLength, 1, fp) != 1 ||
        memcmp(storedTitle, title, (size_t)storedLength) != 0) {
        fprintf(stderr, "The inserted record is incorrect in the data file\n");
        if (fp != NULL) fclose(fp);
        return 1;
    }
    fclose(fp);

    dataOffset = DATA_HEADER_SIZE;
    if (!findKey(key, indexName, &foundOffset) || foundOffset != dataOffset) {
        fprintf(stderr, "The inserted key is missing from the index\n");
        return 1;
    }

    remove(dataName);
    remove(indexName);
    printf("* checkAddTableEntry: OK\n");
    return 0;
}
