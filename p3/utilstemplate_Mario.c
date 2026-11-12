#include "utils.h"

int no_deleted_registers = NO_DELETED_REGISTERS;

void replaceExtensionByIdx(const char *fileName, char * indexName) {
    return;
}


bool createTable(const char * tableName) {
    return true;
    }

bool createIndex(const char *indexName) {

    FILE *fi = NULL;
    char newname[len(indexName)];
    char ext[5] = ".idx";

    if(!indexName){
        return false;
    }

    if(!(fi = (FILE*)fopen(indexName, "rb"))){
        if(!(fi = (FILE*)fopen(indexName, "wb"))){
            return false;
        }
        

    }

    return true;
}
void printnode(size_t _level, size_t level,
               FILE * indexFileHandler, int node_id, char side) {
    return;
}

void printTree(size_t level, const char * indexName)
{
    return;
}

bool findKey(const char * book_id, const char *indexName,
             int * nodeIDOrDataOffset)
 {
     return true;
 }

bool addIndexEntry(char * book_id,  int bookOffset, char const * indexName) {
    return true;
}

bool addTableEntry(Book * book, const char * dataName,
                   const char * indexName) {
    return true;
}