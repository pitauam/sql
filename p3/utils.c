#include "utils.h"

int no_deleted_registers = NO_DELETED_REGISTERS;

void replaceExtensionByIdx(const char *fileName, char * indexName) {
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
void printnode(size_t _level, size_t level, FILE *indexFileHandler, int node_id, char side) {
    
    /*Comprobamos errores de entrada*/
    if (!indexFileHandler || node_id == -1 || _level > level) {
        return;
    }

    Node node;
    size_t i;
    long offset = (long)node_id * sizeof(Node) + INDEX_HEADER_SIZE;

    /*Nos posicionamos en el nodo*/
    if (fseek(indexFileHandler, offset, SEEK_SET) != 0) {
        return;
    }
    /*Sacamos la informacion del nodo*/
    if (fread(&node, sizeof(Node), 1, indexFileHandler) != 1) {
        return;
    }

    for (i = 0; i < _level; i++) {
        printf("    ");
    }

    if (_level == 0) {
        printf("%.4s (%d): %d\n", node.book_id, node_id, node.offset);
    } else {
        printf("%c %.4s (%d): %d\n", side, node.book_id, node_id, node.offset);
    }

    /*hacemos recursividad*/
    printnode(_level + 1, level, indexFileHandler, node.left, 'l');
    printnode(_level + 1, level, indexFileHandler, node.right, 'r');

    return;
}

void printTree(size_t level, const char *indexName) {
    if (!indexName) {
        return;
    }

    FILE *fi = fopen(indexName, "rb");
    if (!fi) {
        return;
    }

    int raiz;
    if (fread(&raiz, sizeof(int), 1, fi) != 1) {
        fclose(fi);
        return;
    }

   /*comprobamos si esta vacio*/
    if (raiz == -1) {
        fclose(fi);
        return;
    }

    /*imprimimos el primer nodo llamando a printnode*/
    printnode(0, level, fi, raiz, ' ');

    fclose(fi);

    return;
}

/*nodeIDOrDataOffset es el id del ultimo nodo visitado si no se encuentra la clave (el punto de inserción de la clave buscada); y el offset del registro si se encuentra*/
bool findKey(const char * book_id, const char *indexName,
             int * nodeIDOrDataOffset)
{
    FILE *pF;

    int root;
    int current;
    int comparison;
    long fileSize;
    long nodeCount;
    long visited;
    long nodePosition;
    Node node;

    if (!book_id || !indexName || !nodeIDOrDataOffset){
        return false;
    }

    /*por defecto se pone a -1*/
    *nodeIDOrDataOffset = -1;
    pF = fopen(indexName, "rb");
    if (!pF){
        return false;
    }

    /*se comprueba que hay un header*/
    if (fseek(pF, 0L, SEEK_END) != 0){
        fclose(pF);
        return false;
    }
    
    /*tamaño del archivo*/
    fileSize = ftell(pF);
    /*comprueba que el nodo tenga el tamaño valido*/
    if (fileSize < INDEX_HEADER_SIZE || (fileSize - INDEX_HEADER_SIZE) % (long)sizeof(Node) != 0){
        fclose(pF);
        return false;
    }

    nodeCount = (fileSize - INDEX_HEADER_SIZE) / (long)sizeof(Node);

    if (fseek(pF, 0L, SEEK_SET) != 0 || fread(&root, sizeof(root), 1, pF) != 1){
        fclose(pF);
        return false;
    }

    /*si la raiz es -1, el arbol está vacío*/
    if (root == NO_DELETED_REGISTERS){
        fclose(pF);
        return false;
    }

    current = root;
    visited = 0;
    while (current != NO_DELETED_REGISTERS)
    {
        if (current < 0 || (long)current >= nodeCount || visited >= nodeCount){
            fclose(pF);
            return false;
        }

        nodePosition = INDEX_HEADER_SIZE + (long)current * (long)sizeof(Node);
        if (fseek(pF, nodePosition, SEEK_SET) != 0 || fread(&node, sizeof(node), 1, pF) != 1){
            fclose(pF);
            return false;
        }

        visited++;

        comparison = memcmp(book_id, node.book_id, PK_SIZE);
        if (comparison == 0){
            *nodeIDOrDataOffset = node.offset;
            fclose(pF);
            return true;
        }

        /*si no está se devuelve el ultimo nodo que se ha vistiado*/
        *nodeIDOrDataOffset = current;
        if (comparison < 0){
            current = node.left;
        } else {
            current = node.right;
        }
    }

    fclose(pF);
    return false;
}

bool addIndexEntry(char * book_id,  int bookOffset, char const * indexName) {
    if (!book_id || !indexName) {
        return false;
    }

    int parentID = -1;
    long offset = PK_SIZE + 4 * sizeof(int);

    /* Comprobamos si la clave ya existe y obtenemos su futuro nodo padre */
    if (findKey(book_id, indexName, &parentID)) {
        return false; /* ya existe */
    }

    FILE *fp = fopen(indexName, "r+b");
    if (!fp) {
        return false;
    }

    /* Leemos la cabecera del índice */
    int root = -1;
    int deleted = -1;
    fread(&root, sizeof(int), 1, fp);
    fread(&deleted, sizeof(int), 1, fp);

    /* Miramos en qué posición se escribirá el nuevo nodo */
    int newNodeID = -1;
    if (deleted != NO_DELETED_REGISTERS) {
        /* Reutilizamos el primer nodo de la lista de borrados */
        newNodeID = deleted;
        long deletedOffset = INDEX_HEADER_SIZE + (long)newNodeID * offset;
        
        /* Leemos el puntero al siguiente nodo borrado (está después de los 4 bytes de PK) */
        int nextDeleted = -1;
        fseek(fp, deletedOffset + PK_SIZE, SEEK_SET);
        fread(&nextDeleted, sizeof(int), 1, fp);
        
        fseek(fp, sizeof(int), SEEK_SET); 
        fwrite(&nextDeleted, sizeof(int), 1, fp);
    } else {
        /* Añadimos al final del fichero */
        fseek(fp, 0, SEEK_END);
        long fileSize = ftell(fp);
        newNodeID = (fileSize - INDEX_HEADER_SIZE) / offset;
    }

    /* Preparamos la estructura del nuevo nodo */
    Node newNode;
    memcpy(newNode.book_id, book_id, PK_SIZE);
    newNode.left = -1;
    newNode.right = -1;
    newNode.parent = parentID;
    newNode.offset = bookOffset;

    /* Escribimos el nuevo nodo en disco */
    long newNodeOffset = INDEX_HEADER_SIZE + (long)newNodeID * offset;
    fseek(fp, newNodeOffset, SEEK_SET);
    fwrite(newNode.book_id, PK_SIZE, 1, fp);
    fwrite(&newNode.left, sizeof(int), 1, fp);
    fwrite(&newNode.right, sizeof(int), 1, fp);
    fwrite(&newNode.parent, sizeof(int), 1, fp);
    fwrite(&newNode.offset, sizeof(int), 1, fp);

    /* Actualizamos el árbol */
    if (root == -1) {
        /* Como está vacío pasa a ser la raíz */
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

        /* Comparamos las claves */
        if (strncmp(book_id, parentNode.book_id, PK_SIZE) < 0) {
            parentNode.left = newNodeID;
        } else {
            parentNode.right = newNodeID;
        }

        /* Reescribimos los punteros left y right */
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