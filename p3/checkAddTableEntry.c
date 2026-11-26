/*
 * Created for the addTableEntry test.
 */
#include "tester.h"

void checkAddTableEntry(const char *dataName, const char *indexName)
/**
 * check addTableEntry
 */
{
    const char title[] = "Test title";
    const char secondTitle[] = "Second title";
    Book book;
    FILE *fp;
    int header = NO_DELETED_REGISTERS;
    int titleLength;
    int storedLength;
    int foundOffset;
    char key[PK_SIZE];
    char storedTitle[128];

    remove(dataName);
    remove(indexName);

    fp = fopen(dataName, "wb");
    if (fp == NULL || fwrite(&header, sizeof(header), 1, fp) != 1) {
        fprintf(stderr,
                "Error in addTableEntry: could not initialize data file\n");
        if (fp != NULL)
            fclose(fp);
        exit(EXIT_FAILURE);
    }
    fclose(fp);

    fp = fopen(indexName, "wb");
    if (fp == NULL || fwrite(&header, sizeof(header), 1, fp) != 1 ||
        fwrite(&header, sizeof(header), 1, fp) != 1) {
        fprintf(stderr,
                "Error in addTableEntry: could not initialize index file\n");
        if (fp != NULL)
            fclose(fp);
        exit(EXIT_FAILURE);
    }
    fclose(fp);

    memcpy(key, "ABCD", PK_SIZE);
    memcpy(book.book_id, key, PK_SIZE);
    book.title_len = strlen(title);
    book.title = (char *)title;

    if (!addTableEntry(&book, dataName, indexName)) {
        fprintf(stderr,
                "Error in addTableEntry: first insertion failed\n");
        exit(EXIT_FAILURE);
    }

    /* 1) add existing key should complain */
    if (addTableEntry(&book, dataName, indexName)) {
        fprintf(stderr,
                "Error in addTableEntry: inserted existing key %.*s\n",
                PK_SIZE, book.book_id);
        exit(EXIT_FAILURE);
    }

    /* 2) check the first record stored in the data file */
    fp = fopen(dataName, "rb");
    titleLength = (int)strlen(title);
    storedLength = 0;
    memset(storedTitle, '\0', sizeof(storedTitle));

    if (fp == NULL || fseek(fp, DATA_HEADER_SIZE, SEEK_SET) != 0 ||
        fread(book.book_id, PK_SIZE, 1, fp) != 1 ||
        fread(&storedLength, sizeof(storedLength), 1, fp) != 1 ||
        storedLength != titleLength ||
        fread(storedTitle, (size_t)storedLength, 1, fp) != 1 ||
        memcmp(storedTitle, title, (size_t)titleLength) != 0) {
        fprintf(stderr,
                "Error in addTableEntry: inserted data record is incorrect\n");
        if (fp != NULL)
            fclose(fp);
        exit(EXIT_FAILURE);
    }
    fclose(fp);

    /* 3) findKey should return the offset of the new record */
    foundOffset = -1;
    if (!findKey(key, indexName, &foundOffset) ||
        foundOffset != DATA_HEADER_SIZE) {
        fprintf(stderr,
                "Error in addTableEntry: inserted key is missing from index\n");
        exit(EXIT_FAILURE);
    }

    /* 4) insert a second record and verify that it can be found */
    memcpy(key, "EFGH", PK_SIZE);
    memcpy(book.book_id, key, PK_SIZE);
    book.title_len = strlen(secondTitle);
    book.title = (char *)secondTitle;

    if (!addTableEntry(&book, dataName, indexName)) {
        fprintf(stderr,
                "Error in addTableEntry: second insertion failed\n");
        exit(EXIT_FAILURE);
    }

    foundOffset = -1;
    if (!findKey(key, indexName, &foundOffset)) {
        fprintf(stderr,
                "Error in addTableEntry: second key is missing from index\n");
        exit(EXIT_FAILURE);
    }

    remove(dataName);
    remove(indexName);
    printf("* checkAddTableEntry: OK\n");
}
