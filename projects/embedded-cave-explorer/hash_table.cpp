// ============================================
// The HashTable class file
//
// Copyright 2025 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================

/****************************************************************************
* Include the Public Interface
*
* By including the public interface at the top of the file, the compiler can
* enforce that the function declarations in the the header are not in
* conflict with the definitions in the file. This is not a guarantee of
* correctness, but it is better than nothing!
***************************************************************************/
#include "hash_table.h"


/****************************************************************************
* Include other private dependencies
*
* These other modules are used in the implementation of the hash table module,
* but are not required by users of the hash table.
***************************************************************************/
#include <cstdlib>    // For nullptr and exit
#include <cstdio>     // For printf
#include <new>        // For new/delete


/****************************************************************************
* Hidden Definitions
*
* These definitions are not available outside of this file. However, because
* the are forward declared in hash_table.h, the type names are
* available everywhere and user code can hold pointers to these structs.
***************************************************************************/
/**
 * This structure represents an a hash table.
 */
struct _HashTable {
  /** The array of pointers to the head of a singly linked list, whose nodes
      are HashTableEntry objects */
  HashTableEntry** buckets;

  /** The hash function pointer */
  HashFunction hash;

  /** The number of buckets in the hash table */
  unsigned int num_buckets;
};

/**
 * This structure represents a hash table entry.
 */
struct _HashTableEntry {
  /** The key for the hash table entry */
  unsigned int key;

  /** The value associated with this hash table entry */
  void* value;

  /**
  * A pointer pointing to the next hash table entry
  * nullptr means there is no next entry (i.e. this is the tail)
  */
  HashTableEntry* next;
};


/****************************************************************************
* Private Functions
*
* These functions are not available outside of this file, since they are not
* declared in hash_table.h.
***************************************************************************/
/**
* createHashTableEntry
*
* Helper function that creates a hash table entry by allocating memory for it on
* the heap. It initializes the entry with key and value, initialize pointer to
* the next entry as nullptr, and return the pointer to this hash table entry.
*
* @param key The key corresponds to the hash table entry
* @param value The value stored in the hash table entry
* @return The pointer to the hash table entry
*/
static HashTableEntry* createHashTableEntry(unsigned int key, void* value) {
  HashTableEntry* newEntry = new HashTableEntry;
  newEntry->key = key;
  newEntry->value = value;
  newEntry->next = nullptr;

  return newEntry;
}

/**
* findItem
*
* Helper function that checks whether there exists the hash table entry that
* contains a specific key.
*
* @param myHashTable The pointer to the hash table.
* @param key The key corresponds to the hash table entry
* @return The pointer to the hash table entry, or nullptr if key does not exist
*/
static HashTableEntry* findItem(HashTable* myHashTable, unsigned int key) {
  unsigned int bucketNum = myHashTable->hash(key);
  HashTableEntry* current = myHashTable->buckets[bucketNum];

  while (current != nullptr) {
    if (current->key == key) {
      return current;
    }
    current = current->next;
  }

  return nullptr;
}

/****************************************************************************
* Public Interface Functions
*
* These functions implement the public interface as specified in the header
* file, and make use of the private functions and hidden definitions in the
* above sections.
****************************************************************************/
HashTable* createHashTable(HashFunction myHashFunc, unsigned int numBuckets) {
  // The hash table has to contain at least one bucket. Exit gracefully if
  // this condition is not met.
  if (numBuckets == 0) {
    printf("Hash table has to contain at least 1 bucket...\n");
    exit(1);
  }

  // Allocate memory for the new HashTable struct on heap.
  HashTable* newTable = new HashTable;

  // Initialize the components of the new HashTable struct.
  newTable->hash = myHashFunc;
  newTable->num_buckets = numBuckets;
  newTable->buckets = new HashTableEntry*[numBuckets];

  // Initialize each bucket as nullptr.
  for (unsigned int i = 0; i < numBuckets; ++i) {
    newTable->buckets[i] = nullptr;
  }

  // Return the newly created hash table.
  return newTable;
}

void destroyHashTable(HashTable* myHashTable) {
  // Loop through each bucket of the hash table to remove all items.
  for (unsigned int i = 0; i < myHashTable->num_buckets; ++i) {
    HashTableEntry* current = myHashTable->buckets[i];

    // Delete all entries in this bucket
    while (current != nullptr) {
      HashTableEntry* next = current->next;
      delete static_cast<char*>(current->value);  // Free the value
      delete current;  // Free the entry
      current = next;
    }
  }

  // Free buckets array
  delete[] myHashTable->buckets;

  // Free hash table
  delete myHashTable;
}

void* insertItem(HashTable* myHashTable, unsigned int key, void* value) {
  // First, we want to check if the key is present in the hash table.
  HashTableEntry* existingEntry = findItem(myHashTable, key);

  if (existingEntry != nullptr) {
    // The key is present in the hash table - update and return old value.
    void* oldValue = existingEntry->value;
    existingEntry->value = value;
    return oldValue;
  } else {
    // The key is not present - insert new entry at head of bucket.
    unsigned int bucketNum = myHashTable->hash(key);
    HashTableEntry* newEntry = createHashTableEntry(key, value);
    newEntry->next = myHashTable->buckets[bucketNum];
    myHashTable->buckets[bucketNum] = newEntry;
    return nullptr;  // Return nullptr as nothing is overwritten.
  }
}

void* getItem(HashTable* myHashTable, unsigned int key) {
  // Check if the key is present in the hash table.
  HashTableEntry* entry = findItem(myHashTable, key);

  return (entry != nullptr) ? entry->value : nullptr;
}

void* removeItem(HashTable* myHashTable, unsigned int key) {
  unsigned int bucketNum = myHashTable->hash(key);
  HashTableEntry* current = myHashTable->buckets[bucketNum];
  HashTableEntry* prev = nullptr;

  // Search for the entry with the matching key
  while (current != nullptr && current->key != key) {
    prev = current;
    current = current->next;
  }

  // If the key is not present in the list
  if (current == nullptr) {
    return nullptr;
  }

  // Unlink the node from the list
  if (prev == nullptr) {
    // The node to remove is the head of the bucket
    myHashTable->buckets[bucketNum] = current->next;
  } else {
    // The node to remove is in the middle or end
    prev->next = current->next;
  }

  // Store the old value, free the entry, and return the value
  void* oldValue = current->value;
  delete current;

  return oldValue;
}

void deleteItem(HashTable* myHashTable, unsigned int key) {
  // Remove the entry and free the returned data
  void* value = removeItem(myHashTable, key);
  if (value != nullptr) {
    delete static_cast<char*>(value);
  }
}