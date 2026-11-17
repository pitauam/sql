#include "utils.h"
#include <limits.h>
#include <string.h>

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

    /*fseek mueve el cursor al final para saber dónde termina el fichero*/
    if (fseek(pF, 0L, SEEK_END) != 0){
        fclose(pF);
        return false;
    }
    
    /*ftell devuelve la posición del cursor, que aquí coincide con el tamaño*/
    fileSize = ftell(pF);
    /*comprueba que el nodo tenga el tamaño valido*/
    if (fileSize < INDEX_HEADER_SIZE || (fileSize - INDEX_HEADER_SIZE) % (long)sizeof(Node) != 0){
        fclose(pF);
        return false;
    }

    nodeCount = (fileSize - INDEX_HEADER_SIZE) / (long)sizeof(Node);

    /*fseek vuelve al byte 0, donde está guardado el identificador de la raíz*/
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
        /*fseek pone el cursor al comienzo del nodo que queremos leer*/
        if (fseek(pF, nodePosition, SEEK_SET) != 0 || fread(&node, sizeof(node), 1, pF) != 1){
            fclose(pF);
            return false;
        }

        visited++;

        /*memcmp compara exactamente los 4 bytes, sin necesitar un '\0'*/
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
    FILE *dataFile;
    int nodeIDOrDataOffset;
    int deletedHead;
    int nextDeleted;
    int deletedSize;
    int titleLength;
    int leftoverSize;
    int replacementHead;
    long fileSize;
    long deletedOffset;
    long previousDeletedOffset;
    long insertOffset;
    long leftoverOffset;
    long remainingSize;
    long visited;
    long maxDeletedRecords;
    size_t titleLengthSize;
    bool foundSpace;
    bool splitSpace;

    if (!book || !book->title || !dataName || !indexName){
        return false;
    }

    /*la longitud guardada en disco es un int y no incluye el '\0' final*/
    titleLengthSize = strlen(book->title);
    if (titleLengthSize > (size_t)INT_MAX){
        return false;
    }
    titleLength = (int)titleLengthSize;

    /*comprueba que la clave no esté ya en el indice*/
    if (findKey(book->book_id, indexName, &nodeIDOrDataOffset)){
        return false;
    }

    dataFile = fopen(dataName, "r+b");
    if (!dataFile){
        return false;
    }

    /*fseek vuelve al byte 0 para leer el inicio de la cadena de borrados*/
    if (fseek(dataFile, 0L, SEEK_SET) != 0 ||
        fread(&deletedHead, sizeof(deletedHead), 1, dataFile) != 1){
        fclose(dataFile);
        return false;
    }

    /*fseek lleva el cursor al final; ftell devuelve ese offset para poder añadir al final*/
    if (fseek(dataFile, 0L, SEEK_END) != 0){
        fclose(dataFile);
        return false;
    }
    fileSize = ftell(dataFile);
    if (fileSize < DATA_HEADER_SIZE || fileSize > INT_MAX){
        fclose(dataFile);
        return false;
    }

    insertOffset = fileSize;
    foundSpace = false;
    splitSpace = false;

    /*si hay registros borrados, busca el primer hueco donde quepa el título*/
    if (deletedHead != NO_DELETED_REGISTERS){
        deletedOffset = (long)deletedHead;
        previousDeletedOffset = NO_DELETED_REGISTERS;
        visited = 0;
        maxDeletedRecords = (fileSize - DATA_HEADER_SIZE) / (2L * (long)sizeof(int)) + 1;

        while (deletedOffset != NO_DELETED_REGISTERS){
            if (deletedOffset < DATA_HEADER_SIZE ||
                deletedOffset + 2L * (long)sizeof(int) > fileSize ||
                visited >= maxDeletedRecords){
                fclose(dataFile);
                return false;
            }

            /*fseek posiciona el cursor al principio del registro borrado*/
            if (fseek(dataFile, deletedOffset, SEEK_SET) != 0 ||
                fread(&nextDeleted, sizeof(nextDeleted), 1, dataFile) != 1 ||
                fread(&deletedSize, sizeof(deletedSize), 1, dataFile) != 1){
                fclose(dataFile);
                return false;
            }
            visited++;

            if (deletedSize < 0 ||
                deletedOffset + 2L * (long)sizeof(int) + deletedSize > fileSize){
                fclose(dataFile);
                return false;
            }

            if (deletedSize >= titleLength){
                insertOffset = deletedOffset;
                foundSpace = true;
                remainingSize = (long)deletedSize - titleLength;

                /*si queda espacio para otra cabecera, mantiene el resto en la cadena*/
                if (remainingSize >= 2L * (long)sizeof(int)){
                    leftoverOffset = deletedOffset + 2L * (long)sizeof(int) + titleLength;
                    leftoverSize = (int)(remainingSize - 2L * (long)sizeof(int));
                    if (leftoverOffset > INT_MAX){
                        fclose(dataFile);
                        return false;
                    }
                    replacementHead = (int)leftoverOffset;
                    splitSpace = true;
                } else {
                    replacementHead = nextDeleted;
                }
                break;
            }

            if (nextDeleted != NO_DELETED_REGISTERS && nextDeleted < DATA_HEADER_SIZE){
                fclose(dataFile);
                return false;
            }
            previousDeletedOffset = deletedOffset;
            deletedOffset = (long)nextDeleted;
        }
    }

    if (insertOffset > INT_MAX){
        fclose(dataFile);
        return false;
    }

    /*escribe clave, longitud y contenido sin guardar el '\0'*/
    /* fseek coloca el cursor en el hueco elegido o al final del fichero. */
    if (fseek(dataFile, insertOffset, SEEK_SET) != 0 ||
        fwrite(book->book_id, PK_SIZE, 1, dataFile) != 1 ||
        fwrite(&titleLength, sizeof(titleLength), 1, dataFile) != 1 ||
        (titleLength > 0 && fwrite(book->title, (size_t)titleLength, 1, dataFile) != 1)){
        fclose(dataFile);
        return false;
    }

    if (foundSpace){
        /*si se dividió el hueco, crea la cabecera de la parte que queda libre*/
        if (splitSpace){
            /*fseek lleva el cursor al comienzo del espacio libre restante*/
            if (fseek(dataFile, leftoverOffset, SEEK_SET) != 0 ||
                fwrite(&nextDeleted, sizeof(nextDeleted), 1, dataFile) != 1 ||
                fwrite(&leftoverSize, sizeof(leftoverSize), 1, dataFile) != 1){
                fclose(dataFile);
                return false;
            }
        }

        /*actualiza el enlace anterior o la cabecera de la tabla*/
        if (previousDeletedOffset == NO_DELETED_REGISTERS){
            /*fseek vuelve a la cabecera para actualizar el primer borrado*/
            if (fseek(dataFile, 0L, SEEK_SET) != 0 ||
                fwrite(&replacementHead, sizeof(replacementHead), 1, dataFile) != 1){
                fclose(dataFile);
                return false;
            }
        } else {
            /*fseek coloca el cursor en el enlace del registro borrado anterior*/
            if (fseek(dataFile, previousDeletedOffset, SEEK_SET) != 0 ||
                fwrite(&replacementHead, sizeof(replacementHead), 1, dataFile) != 1){
                fclose(dataFile);
                return false;
            }
        }
    }

    if (fclose(dataFile) != 0){
        return false;
    }

    /*actualiza el índice con el offset donde se guardó el registro*/
    return addIndexEntry(book->book_id, (int)insertOffset, indexName);
}
