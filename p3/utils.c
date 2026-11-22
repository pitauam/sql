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
    return true;
}

bool addTableEntry(Book * book, const char * dataName,
                   const char * indexName) {
    return true;
}