/* 
 * chainedHashTable.c
 *
 * Implementation of abstractHashTable.h that uses a chained
 * hash table. The calling program supplies the hashing function
 * as part of the initialization.
 *
 * Limitation: this implementation will not work well for duplicate
 * keys. It will store both but the returned data is unpredictable.
 *
 * Limitation: this implementation assumes keys are strings 
 * less than 128 chars long
 *
 *   Created by Sally Goldin on 5 March 2012 for CPE 113
 *   Updated to explicity keep track of list head and tail on
 *       26 March 2013
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abstractHashTable.h"

#define KEYLEN 128
#define BUCKET_COUNT 20000000

/* Structure for table elements */
typedef struct _hashItem
{
    //char key[KEYLEN];              /* copy of the key */
    char data[128];                    /* data */
    struct _hashItem * next;       /* next item in the bucket if any */
} HASH_ITEM_T; 

/* Structure for list with head and tail */
typedef struct _linkedList
{
    HASH_ITEM_T* head;             /* first item in list - null if list empty*/
    HASH_ITEM_T* tail;             /* last item in the list */
} LINKED_LIST_T; 


/* Hash function - set by hashTableInit  */
unsigned int (*hashFn)(char* key) = NULL;

static LINKED_LIST_T * table = NULL;  /* we will allocate our table based on
                                       * initialization arguments and store it
                                       * here 
                                       */     
static int tableSize = 0;     /* size of the table */
static int itemCount = 0;     /* keep track of current number of stored items */

/* Return the number of slots in the hash table.
 */
int hashTableSize()
{
    return tableSize;
}


/* Return the number of items currently stored in the hash table.
 */
int hashTableItemCount()
{
    return itemCount;
}
 
/* Initialize the hash table.
 * Arguments
 *    size                - How many slots in the table
 *                          Must be 1 or greater. We assume the caller
 *                          has checked this.
 *    hashFunction        - Function that takes a string and returns an int
 *                          which will be the index into the table.
 * Return 1 if successful, 0 if some error occurred.
 */
int hashTableInit(int size, unsigned int (*hashFunction)(char* key))
{
    int bOk = 1;
    /* free the old table, if any */
    hashTableFree();
    hashFn = hashFunction;
    tableSize = size;
    /* try to allocate the table, which will store pointers
     * to LINKED_LIST_T elements.
     */
    table = (LINKED_LIST_T*) calloc(size,sizeof(LINKED_LIST_T));
    if (table == NULL)
       {
       bOk = 0;
       }
    return bOk;
}


/* Free the hash table.
 */
void hashTableFree()
{
    int i = 0;
    HASH_ITEM_T * pItem = NULL;
    HASH_ITEM_T * pNextItem = NULL;
    if (table != NULL) 
        {
	for (i = 0; i < tableSize; i++)
	   {
	   if (table[i].head != NULL)  /* something stored in this slot */
	       {
	       pItem = table[i].head;
	       /* walk the linked list, freeing each item */
	       while (pItem != NULL)
	          {
		  pNextItem = pItem->next;
                  free(pItem);
		  pItem = pNextItem;
	          }
	       table[i].head = NULL;
               table[i].tail = NULL;
	       }
	   }
	free(table);
	table = NULL;
	tableSize = 0;
	itemCount = 0;
        }
}


/* Insert a value into the hash table.
 * Arguments 
 *    key                 - character string key
 *    data                - data to store in the table
 *    pCollision          - set to true if there was a collision storing
 *                          the data, else false 
 * Returns true (1) unless hash table has not been initialized or
 * we can't allocate memory, in which case returns false (0)
 */
int hashTableInsert(char* key, int* pCollision)
{
	int bOk = 1;
	int hashval = 0;
	HASH_ITEM_T * pItem = NULL;
	HASH_ITEM_T * pTemp = NULL;
	if (table == NULL)  /* not initialized */
		return 0;
	pItem = (HASH_ITEM_T*) calloc(1,sizeof(HASH_ITEM_T));
	if (pItem == NULL)
		{
		bOk = 0;  /* can't allocate memory */
		}
	else
		{
		strncpy(pItem->data,key, KEYLEN-1);
		hashval = hashFn(key);
		if (table[hashval].head == NULL)
			{
			table[hashval].head = pItem;  /* bucket was empty */
			*pCollision = 0;              /* no collision */
			}
		else
			{
			*pCollision = 1;         /* We have a collision */
			/* put the new item at the end of the bucket list */
			table[hashval].tail->next = pItem;
			}
		table[hashval].tail = pItem;
		itemCount++;
		}
	return bOk;
}


/* Look up a value in the hash table.
 * Arguments 
 *    key                 - character string key
 * Returns the data associated with the key, or NULL if 
 * data associated with the key is not found.
 */
void* hashTableLookup(char* key)
{
    /* This function is similar to remove but we do not
     * change anything in the hashtable structure 
     */
    void * foundData = NULL;
    HASH_ITEM_T* pPrev = NULL;
    HASH_ITEM_T* pTemp = NULL;
    if (table != NULL)    /* initialized */
       {
       int hashval = hashFn(key);
       if (table[hashval].head != NULL)   /* in the table */
           {
	   pTemp = table[hashval].head;
	   while (pTemp != NULL)
	       {
	       if (strcmp(pTemp->data,key/*,KEYLEN-1*/) == 0)  /* match */
		  {
		  foundData = pTemp->data;
		  pTemp = NULL;  /* this will make us exit loop */
		  }
	       else
	          {
		  pPrev = pTemp;
		  pTemp = pTemp->next;  /* check next item */	  
		  }
	       } /* end loop through items in the bucket */
           }        /* end if the key is in the table */
       }            /* end if the hash table is initialized */            
    return foundData;
}


void printTab()
{
	int i;
	HASH_ITEM_T* pTemp = NULL;

	for(i = 0; i < BUCKET_COUNT; i++)
	{
		pTemp = table[i].head;
		while(pTemp)
		{
			printf("%s\n", pTemp->data);
			pTemp = pTemp->next;
		}
	}
}

/* Robust hash function that uses bitwise operations to
 * modify string values. Adapted from Kyle Loudon,
 * "Mastering Algorithms with C"
 */
unsigned int bitwiseOpHash(char* key)
{
    unsigned int result = 0;
    unsigned int tmp = 0;
    int size = hashTableSize();
    int i = 0;
    for (i = 0; i < strlen(key); i++)
        {
		/* shift up four bits then add in next char */ 
		result = (result << 4) + key[i];
        if (tmp = (result & 0xf0000000))  /* if high bit is set */
	    	{
	   		/* XOR result with down shifted tmp */
	    	result = result ^ (tmp >> 24);
            /* then XOR with tmp itself */
	    	result = result ^ tmp;
	    	}
        }
    result = result % size;   /* make it fit in the table size */ 
    return result;
}


int buildDictionary()
{
	FILE* wordlist = fopen("dictionary.txt","r");
	int pCollision = 0;
	char word[32];
	char word2[32];
	int nb_col = 0;
	int i;

	hashTableInit(BUCKET_COUNT, &bitwiseOpHash);
	while (fgets(word, sizeof(word), wordlist))
	    {
	    int len = strlen(word);
		//if (word[len - 1] == '\n')
		    //word[len - 1] = '\0';
		for(i = 0; i < len - 2; i++)
			word2[i] = word[i];
		word2[len - 2] = '\0';
		hashTableInsert(word2, &pCollision);
		nb_col += pCollision;
		}
	fclose(wordlist);
	return 1;
}





