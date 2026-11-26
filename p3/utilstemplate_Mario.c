#include <string.h>
#include "utils.h"


int no_deleted_registers = NO_DELETED_REGISTERS;

void replaceExtensionByIdx(const char *fileName, char *indexName) {
    if (!fileName || !indexName) {
        return;
    }

    size_t len = strlen(fileName);
    if (len < 4) {
        return;
    }

    strcpy(indexName, fileName);
    strcpy(&indexName[len - 4], ".idx");
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

    /* llamamos a replaceExtension*/
    replaceExtensionByIdx(indexName, newname);

    if(!newname){
        return false;
    }

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

    Node *node = NULL;
    size_t i ;
    long offset = (long)node_id * sizeof(Node) + INDEX_HEADER_SIZE;

    

    if(fseek(indexFileHandler, offset, SEEK_SET) != 0){
        return;
    }

    if(fread(node, sizeof(Node), 1, indexFileHandler) != 1){
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

    printnode(level, _level -1, indexFileHandler, node->left, 'l');
    printnode(level, _level -1, indexFileHandler, node->right, 'r');

    fclose(indexFileHandler);
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

    printnode(level, level, fi, raiz, "");

    fclose(fi);

    return;
}

bool findKey(const char * book_id, const char *indexName,
             int * nodeIDOrDataOffset)
 {
     return true;
 }

bool addIndexEntry(char *book_id, int bookOffset, const char *indexName) {


    int parentID = -1;
    long offset = PK_SIZE + 4 * sizeof(int);

    /*Comprobamos si la clave ya existe y obtenemos su futuro nodo padre */
    if (findKey(book_id, indexName, &parentID)) {
        return false; /*ya existe*/
    }


    
    FILE *fp = fopen(indexName, "r+b");
    if (!fp) {
        return false;
    }

    /* Leemos la cabecera del índice*/
    int root = -1;
    int deleted = -1;
    fread(&root, sizeof(int), 1, fp);
    fread(&deleted, sizeof(int), 1, fp);

    /*Miramos en qué posición se escribirá el nuevo nodo */
    int newNodeID = -1;
    if (deleted != NO_DELETED_REGISTERS) {
        /* Reutilizamos el primer nodo de la lista de borrados */
        newNodeID = deleted;
        long deletedOffset = INDEX_HEADER_SIZE + (long)newNodeID * offset;
        
        /* Leemos el puntero al siguiente nodo borrado y actualizamos la cabecera */
        int nextDeleted = -1;
        fseek(fp, deletedOffset, SEEK_SET);
        fread(&nextDeleted, sizeof(int), 1, fp);
        
        fseek(fp, sizeof(int), SEEK_SET); 
        fwrite(&nextDeleted, sizeof(int), 1, fp);
    } else {
        /* Añadimos al final del fichero */
        fseek(fp, 0, SEEK_END);
        long fileSize = ftell(fp);
        newNodeID = (fileSize - INDEX_HEADER_SIZE) / offset;
    }

    /*Preparamos la estructura del nuevo nodo */
    Node newNode;
    memcpy(newNode.book_id, book_id, PK_SIZE);
    newNode.left = -1;
    newNode.right = -1;
    newNode.parent = parentID;
    newNode.offset = bookOffset;

    /*Escribimos el nuevo nodo en disco */
    long newNodeOffset = INDEX_HEADER_SIZE + (long)newNodeID * offset;
    fseek(fp, newNodeOffset, SEEK_SET);
    fwrite(newNode.book_id, PK_SIZE, 1, fp);
    fwrite(&newNode.left, sizeof(int), 1, fp);
    fwrite(&newNode.right, sizeof(int), 1, fp);
    fwrite(&newNode.parent, sizeof(int), 1, fp);
    fwrite(&newNode.offset, sizeof(int), 1, fp);

    /*Actualizamos el árbol*/
    if (root == -1) {
        /*Como esta vacio para a ser la reiz*/
        fseek(fp, 0, SEEK_SET);
        fwrite(&newNodeID, sizeof(int), 1, fp);
    } else {
        /* Lee el nodo padre para enlazarlo con el nuevo hijo */
        Node parentNode;
        long parentOffset = INDEX_HEADER_SIZE + (long)parentID * offset;
        fseek(fp, parentOffset, SEEK_SET);
        fread(parentNode.book_id, PK_SIZE, 1, fp);
        fread(&parentNode.left, sizeof(int), 1, fp);
        fread(&parentNode.right, sizeof(int), 1, fp);
        fread(&parentNode.parent, sizeof(int), 1, fp);
        fread(&parentNode.offset, sizeof(int), 1, fp);

        /* Comparamos las claves*/
        if (strncmp(book_id, parentNode.book_id, PK_SIZE) < 0) {
            parentNode.left = newNodeID;
        } else {
            parentNode.right = newNodeID;
        }

        /* Reescribimos los punteros left y right*/
        fseek(fp, parentOffset + PK_SIZE, SEEK_SET);
        fwrite(&parentNode.left, sizeof(int), 1, fp);
        fwrite(&parentNode.right, sizeof(int), 1, fp);
    }

    fclose(fp);
    return true;
}

bool addTableEntry(Book * book, const char * dataName,
                   const char * indexName) {
    return true;
}