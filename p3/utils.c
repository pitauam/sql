#include "utils.h"

int no_deleted_registers = NO_DELETED_REGISTERS;

void replaceExtensionByIdx(const char *fileName, char * indexName) {
    return;
}


/*tableName es el nombre del fichero*/
bool createTable(const char * tableName) {
    FILE *pF = NULL;
    int number = -1;

    if (!tableName){
        return false;
    }

    /*intenta abrirlo*/
    pF = (FILE*)fopen(tableName, "rb");
    /*si no existe lo crea*/
    if (!pF){

        pF = (FILE*)fopen(tableName, "wb");
        if (!pF){
            return false;
        }
        /*inicializa la cabecera a -1, en binario*/
        if (fwrite(&number, sizeof(int), 1, pF) != 1){
            fclose(pF);
            return false;
        }
        
        /*crea fichero de indices si es necesario*/
        createIndex(tableName);
        fclose(pF);
        return true;
    }

    fclose(pF);

    return true;
    }

bool createIndex(const char *indexName) {
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