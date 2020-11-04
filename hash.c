#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

struct Performance *newPerformance() {
    struct Performance *pIndicator = malloc(sizeof(struct Performance));
    if(pIndicator == NULL) {
        fprintf(stderr, "Malloc failed");
        exit(0);
    }
    pIndicator->reads = 0;
    pIndicator->writes = 0;
    pIndicator->mallocs = 0;
    pIndicator->frees = 0;
    return pIndicator;
}

struct HashTable *createTable(struct Performance *performance, unsigned int capacity, int(*hash)(void*, int), int(*compar)(const void*, const void*)) {
    struct HashTable *temp = malloc(sizeof(struct HashTable));
    if(temp == NULL) {
        fprintf(stderr, "createTable malloc failed");
        exit(0);
    }
    temp->capacity = capacity;
    temp->hash = hash;
    temp->compar = compar;
    temp->nel = 0;
    temp->data = malloc(sizeof(void*) * capacity);
    if(temp->data == NULL) {
        fprintf(stderr, "temp->data malloc failed");
        exit(0);
    }
    for(int i = 0; i < capacity; i++) {
        (temp->data)[i] = NULL; 
        performance->reads += 1;
    }
    performance->mallocs += 1;
    return temp;
}

void addElement(struct Performance *performance, struct HashTable *table, void *src) {
    if(table->nel == table->capacity) {
        fprintf(stderr, "max capacity");
        exit(0);
    }
    int index = table->hash(src, table->capacity);
    for(int i = index; i < table->capacity; i++) {
        performance->reads += 1;
        if((table->data)[i] == NULL) {
            (table->data)[i] = src;
            table->nel += 1;
            performance->writes += 1;
            break;
        }
        if(i == (table->capacity - 1)) {
            i = -1;
        }
    }
}

int getIdx(struct Performance *performance, struct HashTable *table, void *src) {
    int index = table->hash(src, table->capacity); 
    int originalIndex = index;
    for(int i = index; i < table->capacity; i++) {  
        if((table->data)[i] != NULL) {
            int comparResult = table->compar(src, (table->data)[i]);
            performance->reads += 1;
            if(comparResult == 0) {
                return i;
            }
            else if(i == (table->capacity - 1)) {
                i = -1;
            }
            else if(i == (originalIndex-1)) { //reaches the last index to search meaning not found
                return -1;
            }
        }
        else {
            return -1;
        }
    }
    return -1; //redundant measure to return -1 if match not found
}

void freeTable(struct Performance *performance, struct HashTable *table) {
    free(table->data);
    free(table);
    performance->frees += 1;
}

void *getElement(struct Performance *performance, struct HashTable *table, void *src) {
    int index = getIdx(performance, table, src);
    if(index == -1) {
        return NULL;
    }
    else {
        return (table->data)[index];
    }
}

void removeElement(struct Performance *performance, struct HashTable *table, void *target) { 
    int removeIndex = getIdx(performance, table, target);
    if(removeIndex != -1) {
        (table->data)[removeIndex] = NULL;
        performance->writes += 1;
    }
}

int hashAccuracy(struct HashTable *table) {
    int totalCollisions = 0;
    for(int index = 0; index < table->capacity; index++) {
        if((table->data)[index] != NULL) {
            int hashResult = table->hash((table->data)[index], table->capacity);
            if(hashResult == index) {
            }
            else if(hashResult < index) {
                totalCollisions += index - hashResult;
            }
            else if(hashResult > index) {
                totalCollisions += (index - hashResult) + table->capacity;
            }
        }
    }
    return totalCollisions;
}

void rehash(struct HashTable *table) {
    if(table->nel < table->capacity) {  
        for(int index = 0; index < table->capacity; index++) {
            if((table->data)[index] != NULL) {
                int hashResult = table->hash((table->data)[index], table->capacity);

                if((index - hashResult) > 1) { // checks if the gap is greater than 1 --> for elements ahead of hashResult
                    for(int j = hashResult; j < index; j++) {
                        if((table->data)[j] == NULL) {
                            (table->data)[j] = (table->data)[index];
                            (table->data)[index] = NULL;
                            int indexUpdate = j; //new index location
                            
                            if(hashResult != 0 && (table->data)[hashResult-1] == NULL && (indexUpdate - hashResult > 1)) { //gap still greater than one, swap behind if possible
                                (table->data)[hashResult-1] = (table->data)[indexUpdate];
                                (table->data)[indexUpdate] = NULL;
                            }
                            break;
                        }
                    }
                }

                if((index - hashResult) < -1) {
                    for(int k = hashResult; k < table->capacity; k++) {
                        if((table->data)[k] == NULL) {                        
                            (table->data)[k] = (table->data)[index];
                            (table->data)[index] = NULL;
                            int indexUpdate = k;
                            if(hashResult != 0 && (table->data)[hashResult-1] == NULL && (indexUpdate - hashResult < -1)) {
                                (table->data)[hashResult-1] = (table->data)[indexUpdate];
                                (table->data)[indexUpdate] = NULL;
                            }
                            break;
                        }

                        if (k == (table->capacity - 1)) {
                            k = -1;
                        }
                        if(k == (hashResult - 1)) {
                            break;
                        }
                        
                    }
                }

            }
        }
    }
}
