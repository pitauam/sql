#include <string.h>
#include "utils.h"


int no_deleted_registers = NO_DELETED_REGISTERS;

void replaceExtensionByIdx(const char *fileName, char * indexName) {
    return;
}


bool createTable(const char * tableName) {
    return true;
    }

bool createIndex(const char *indexName) {

    if (!indexName) {
        return false;
    }

    size_t len = strlen(indexName);
    if (len < 4) {
        return false;
    }

    FILE *fi = NULL;
    int number = -1;
    char newname[len + 1];

    /* Construimos el nombre con extensión .idx */
    strcpy(newname, indexName);
    strcpy(&newname[len - 4], ".idx");

    /*Comprobamos si el fichero .idx ya está creado */
    if (!(fi = fopen(newname, "rb"))) {
        /* creamos el nuevo fichero */
        if (!(fi = fopen(newname, "wb"))) {
            return false;
        }

        /* Inicializamos la cabecera con dos enteros iguales a -1 */
        if (fwrite(&number, sizeof(int), 1, fi) != 1) {
            fclose(fi);
            return false;
        }

        if (fwrite(&number, sizeof(int), 1, fi) != 1) {
            fclose(fi);
            return false;
        }
    }

    /*Cerramos el fichero */
    fclose(fi);

    return true;
}

void printnode(size_t _level, size_t level, FILE * indexFileHandler, int node_id, char side) {

    if(!indexFileHandler || _level > level){
        return;
    }
    FILE *fi = NULL;
    Node *node = NULL;
    size_t i ;
    long offset = (long)node_id * sizeof(Node) + INDEX_HEADER_SIZE;

    if(!(fi = fopen(indexFileHandler, "rb"))){
        return;
    }

    if(fseek(fi, offset, SEEK_SET) != 0){
        return;
    }

    if(fread(node, sizeof(Node), 1, fi) != 1){
        return;
    }


    for (i = 0; i < _level; i++) {
        printf("    ");
    }


    if(_level == 0){
        printf("%.4s (%d): %d\n", node->book_id, node_id, node->offset);
    } else {
        printf("%c %.4s (%d): %d\n", side, node->book_id, node_id, node->offset);
    }

    printTreeRec(level, _level -1, indexFileHandler, node->left, 'l');
    printTreeRec(level, _level -1, indexFileHandler, node->right, 'r');

    fclose(fi);
    return;

   
}

void printTree(size_t level, const char * indexName){   
    
    if(!indexName){
        return;
    }

    FILE *fi = NULL;
    int i;
    int raiz;

    if(!(fi = fopen(indexName, "rb"))){
        return;
    }

    
    /*leemos la raiz y la guardamos*/
    if (fread(&raiz, sizeof(int), 1, fi) != 1) {
        fclose(fi);
        return;
    }
    /*caso de arbol vacio*/
    if(raiz == -1){
        return;
    }

    printnode(level, level, indexName, raiz, "");

    fclose(fi);

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