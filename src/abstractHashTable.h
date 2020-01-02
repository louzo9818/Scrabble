/* 
 *   abstractHashTable.h
 *
 *   Header file for a hash table that uses strings as keys
 *   and stores pointers to arbitrary structures as values
 *
 *   Created by Sally Goldin on 5 March 2012 for CPE 113
 */
#ifndef ABSTRACTHASHTABLE_H
#define ABSTRACTHASHTABLE_H

/* Return the number of slots in the hash table.
 */
int hashTableSize();

/* Return the number of items currently stored in the hash table.
 */
int hashTableItemCount();


/* Initialize the hash table.
 * Arguments
 *    size                - How many slots in the table
 *                          Must be 1 or greater. We assume the caller
 *                          has checked this.
 *    hashFunction        - Function that takes a string and returns an int
 *                          which will be the index into the table.
 * Returns 1 if successful, 0 if some error occurred.
 */
int hashTableInit(int size, unsigned int (*hashFunction)(char* key));


/* Free the hash table.
 */
void hashTableFree();


/* Insert a value into the hash table.
 * Arguments 
 *    key                 - character string key
 *    data                - data to store in the table
 *    pCollision          - set to true if there was a collision storing
 *                          the data, else false 
 * Returns true (1) unless hash table has not been initialized or
 * we can't allocate memory, in which case returns false (0)
 */
int hashTableInsert(char* key,int* pCollision);


/* Look up a value in the hash table.
 * Arguments 
 *    key                 - character string key
 * Returns the data associated with the key, or NULL if 
 * data associated with the key is not found.
 */
void* hashTableLookup(char* key);

void printTab();

unsigned int bitwiseOpHash(char* key);
int buildDictionary();



#endif
