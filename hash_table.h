#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "utils.h"

#define BASE_TEN 10
#define MAX_COMMAND_SIZE 20000
#define CHARACTER_STRING_SIZE 2
#define FUNCTION_PARAMETERS 3
#define NEW_LINE 13
#define CARRIAGE_RETURN 10
#define DEVICE_CONTROL 17
#define STRING_WHITESPACE " "
#define FILE_READ "r"
#define FILE_APPEND "a"
#define ZERO "0"
#define ALWAYS_TRUE 1
#define COMMAND_ADD "add"
#define COMMAND_ADD_ALLOWED_TOKEN_NUMBER 2
#define COMMAND_REMOVE "remove"
#define COMMAND_REMOVE_ALLOWED_TOKEN_NUMBER 2
#define COMMAND_FIND "find"
#define COMMAND_FIND_ALLOWED_TOKEN_NUMBER 3
#define COMMAND_CLEAR "clear"
#define COMMAND_CLEAR_ALLOWED_TOKEN_NUMBER 1
#define COMMAND_PRINT_BUCKET "print_bucket"
#define COMMAND_PRINT_BUCKET_ALLOWED_TOKEN_NUMBER 3
#define COMMAND_PRINT "print"
#define COMMAND_PRINT_ALLOWED_TOKEN_NUMBER 2
#define COMMAND_RESIZE "resize"
#define COMMAND_RESIZE_ALLOWED_TOKEN_NUMBER 2
#define COMMAND_RESIZE_DOUBLE "double"
#define COMMAND_RESIZE_HALVE "halve"
#define COMMAND_NO_ARGUMENTS 1
#define FILE_NOT_FOUND_EXCEPTION "Failed to open file "
#define MEMORY_ALLOCATION_EXCEPTION "Failed to allocate memory!"
#define TOKEN_NOT_FOUND_EXCEPTION "Failed to split token"
#define ARGUMENT_NUMBER_MISMATCH_EXCEPTION "Number of arguments does not match the function declaration"
#define INVAlID_OPERATION_EXCEPTION "Operation given is not supported"
#define BUCKET_INDEX_INVALID_INTEGER_EXCEPTION "Bucket index is not a valid integer"
#define HASH_TABLE_SIZE_EXCEPTION "Invalid hash table size. Size should be a positive 16 bit number"
#define RESIZE_OPERATION_NOT_SUPPORTED_EXCEPTION "Resize operation not supported"
#define NOT_ENOUGH_ARGUMENTS_EXCEPTION "Argument number exception"
#define TRUE "True"
#define FALSE "False"
#define INVALID_SIZE_EXCEPTION "New size is not valid"

typedef struct Bucket {

	char *word;

	struct Bucket *next;

} Bucket;

typedef struct HashTable {

	unsigned int size;

	Bucket **bucketHead;

} HashTable;

/**
 *	Initialize a hash map by allocating memory. initialHashTableSize is the
 *	size for the array of buckets.
 */
void hash_table_initialize(HashTable *hashTable, int initialHashTableSize)
{
	hashTable->size = initialHashTableSize;
	hashTable->bucketHead = (Bucket **)calloc(initialHashTableSize,
	sizeof(Bucket *));
	DIE(hashTable->bucketHead == NULL, MEMORY_ALLOCATION_EXCEPTION);
}

/**
 *	Write text to a specified file
 *	on disk.
 */
void hash_table_write_file(char *fileName, char *text)
{
	FILE *fp;

	fp = fopen(fileName, FILE_APPEND);
	DIE(fp == NULL, FILE_NOT_FOUND_EXCEPTION);
	fprintf(fp, "%s\n", text);
	fclose(fp);
}

/**
 *	Add word to hash table bucket. If the word
 *	is already in the hash table the function
 *	simply returns;
 */
void hash_table_add(HashTable *hashTable, char *word)
{
	unsigned int bucketIndex;
	Bucket *aux;
	Bucket *newNode;

	bucketIndex = hash(word, hashTable->size);
	aux = hashTable->bucketHead[bucketIndex];

	newNode = (Bucket *)calloc(1, sizeof(Bucket));
	DIE(newNode == NULL, MEMORY_ALLOCATION_EXCEPTION);
	newNode->word = (char *)calloc(strlen(word) + 1, sizeof(char));
	DIE(newNode->word == NULL, MEMORY_ALLOCATION_EXCEPTION);
	strcpy(newNode->word, word);
	newNode->next = NULL;

	if (hashTable->bucketHead[bucketIndex] == NULL) {
		hashTable->bucketHead[bucketIndex] = newNode;
		return;
	}

	while (aux->next != NULL) {

		if (strcmp(aux->word, word) == 0)
			return;

		aux = aux->next;
	}

	if (strcmp(aux->word, word) == 0)
		return;

	aux->next = newNode;

}

/**
 *	Remove a word from the hash table.
 */
void hash_table_remove(HashTable *hashTable, char *word)
{
	unsigned int bucketIndex;
	Bucket *aux;
	Bucket *toRemove;

	bucketIndex = hash(word, hashTable->size);
	aux = hashTable->bucketHead[bucketIndex];

	if (aux == NULL)
		return;

	if (
		aux != NULL
		&& strcmp(aux->word, word) == 0
	) {
		hashTable->bucketHead[bucketIndex] = aux->next;

		free(aux->word);
		free(aux);

		return;
	}

	while (
		aux->next != NULL
		&& strcmp(aux->next->word, word) != 0
	)
		aux = aux->next;

	if (aux->next == NULL)
		return;

	toRemove = aux->next;

	aux->next = toRemove->next;

	free(toRemove->word);
	free(toRemove);
}

/**
 *	Returns true if the word is found in the hashTable
 *	or false if the word is not found in the hashTable.
 *	If outputFile is NULL the output is stdout
 *	else the output is written to the file specified
 */
void hash_table_find(HashTable *hashTable, char *word, char *outputFile)
{
	unsigned int bucketIndex;
	Bucket *aux;

	bucketIndex = hash(word, hashTable->size);
	aux = hashTable->bucketHead[bucketIndex];

	while (aux != NULL) {
		if (strcmp(aux->word, word) == 0) {
			if (outputFile == NULL) {
				printf("%s\n", TRUE);
				return;
			}
			hash_table_write_file(outputFile, TRUE);
			return;
		}
		aux = aux->next;
	}

	if (outputFile == NULL)
		printf("%s\n", FALSE);
	else
		hash_table_write_file(outputFile, FALSE);
}

/**
 *	Deletes all entries of a hash table
 */
void hash_table_clear(HashTable *hashTable)
{
	Bucket *aux;
	Bucket *current;
	unsigned int bucketIndex;

	for (bucketIndex = 0; bucketIndex < hashTable->size; bucketIndex++) {
		aux = hashTable->bucketHead[bucketIndex];

		while (aux != NULL) {
			current = aux;
			aux = aux->next;
			free(current->word);
			free(current);
		}

		hashTable->bucketHead[bucketIndex] = NULL;
	}
}

/**
 *	Print all values from the bucket at bucketIndex.
 *	If outputFile is NULL the output is stdout
 *	else the output is written to the file specified
 */
void hash_table_print_bucket(HashTable *hashTable, unsigned int bucketIndex,
char *outputFile)
{
	int emptyLine;
	Bucket *aux;
	FILE *fp;

	fp = NULL;
	emptyLine = 1;
	aux = hashTable->bucketHead[bucketIndex];

	if (outputFile != NULL) {
		fp = fopen(outputFile, FILE_APPEND);
		DIE(fp == NULL, FILE_NOT_FOUND_EXCEPTION);
	}

	while (aux != NULL) {
		emptyLine = 0;

		if (outputFile != NULL) {
			if (aux->next != NULL)
				fprintf(fp, "%s ", aux->word);
			else
				fprintf(fp, "%s", aux->word);

		} else {
			if (aux->next != NULL)
				printf("%s ", aux->word);
			else
				printf("%s", aux->word);
		}

		aux = aux->next;
	}

	if (emptyLine == 0) {
		if (outputFile != NULL)
			fprintf(fp, "\n");
		else
			printf("\n");
	}

	if (outputFile != NULL)
		fclose(fp);
}

/**
 *	Print all values from the hashMap.
 *	If a bucket is empty nothing is printed.
 *	If outputFile is NULL the output is stdout
 *	else the output is written to the file specified
 */
void hash_table_print(HashTable *hashTable, char *outputFile)
{
	unsigned int bucketIndex;
	int emptyLine;
	FILE *fp;
	Bucket *aux;

	fp = NULL;

	if (outputFile != NULL) {
		fp = fopen(outputFile, FILE_APPEND);
		DIE(fp == NULL, FILE_NOT_FOUND_EXCEPTION);
	}

	for (bucketIndex = 0; bucketIndex < hashTable->size; bucketIndex++) {

		emptyLine = 1;

		if (outputFile == NULL) {
			hash_table_print_bucket(hashTable, bucketIndex,
			outputFile);
		} else {
			aux = hashTable->bucketHead[bucketIndex];

			while (aux != NULL) {

				if (aux->next != NULL)
					fprintf(fp, "%s ", aux->word);
				else
					fprintf(fp, "%s", aux->word);

				aux = aux->next;
				emptyLine = 0;
			}

			if (!emptyLine)
				fprintf(fp, "\n");
		}
	}

	if (outputFile != NULL)
		fclose(fp);

}

/**
 *	Resize the given hashMap.
 *	resizeType = 0 -> Double de size
 *	resizeType = 1 -> Halve the size
 */
void hash_table_resize(HashTable *hashTable, int resizeType)
{
	HashTable newHashTable;
	unsigned int newSize;
	unsigned int bucketIndex;
	Bucket *aux;

	newSize = hashTable->size;

	if (resizeType == 0)
		newSize = 2 * hashTable->size;
	else if (resizeType == 1)
		newSize = hashTable->size / 2;

	if (newSize < 2)
		DIE(1, INVALID_SIZE_EXCEPTION);

	hash_table_initialize(&newHashTable, newSize);

	for (bucketIndex = 0; bucketIndex < hashTable->size; bucketIndex++) {
		aux = hashTable->bucketHead[bucketIndex];

		while (aux != NULL) {
			hash_table_add(&newHashTable, aux->word);
			aux = aux->next;
		}

	}

	hash_table_clear(hashTable);
	free(hashTable->bucketHead);
	hashTable->size = newSize;
	hashTable->bucketHead = newHashTable.bucketHead;
}

/**
 *	Free the memory for the entire bucket
 */
void hash_table_free(HashTable *hashTable)
{
	unsigned int bucketIndex;
	Bucket *aux;
	Bucket *current;

	for (bucketIndex = 0; bucketIndex < hashTable->size; bucketIndex++) {
		aux = hashTable->bucketHead[bucketIndex];

		while (aux != NULL) {
			current = aux;
			aux = aux->next;
			free(current->word);
			free(current);
		}

	}

	free(hashTable->bucketHead);
}
