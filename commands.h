#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash_table.h"

char commandBuffer[MAX_COMMAND_SIZE];

void parse_command(char *command, HashTable *hashTable)
{
	char *token;
	char *separator;
	char **parameters;
	int tokenIndex;
	unsigned int bucketIndex;
	int i;

	separator = (char *)calloc(CHARACTER_STRING_SIZE, sizeof(char));
	parameters = (char **)calloc(FUNCTION_PARAMETERS, sizeof(char *));
	tokenIndex = 0;

	if (
		(int)command[0] == NEW_LINE
		|| (int)command[0] == CARRIAGE_RETURN
	) {
		return;
	}

	strcpy(separator, STRING_WHITESPACE);

	token = strtok(command, separator);
	DIE(token == NULL, TOKEN_NOT_FOUND_EXCEPTION);

	while (token != NULL) {

		if (
			(int)token[strlen(token) - 1] == NEW_LINE
			|| (int)token[strlen(token) - 1] == CARRIAGE_RETURN
		) {
			token[strlen(token) - 1] = 0;
		}

		parameters[tokenIndex] = (char *)calloc(strlen(token)+1,
		sizeof(char));
		strcpy(parameters[tokenIndex], token);
		token = strtok(NULL, separator);
		tokenIndex++;
	}

	if (strcmp(parameters[0], COMMAND_ADD) == 0) {

		DIE(tokenIndex != COMMAND_ADD_ALLOWED_TOKEN_NUMBER,
		ARGUMENT_NUMBER_MISMATCH_EXCEPTION);

		hash_table_add(hashTable, parameters[1]);

	} else if (strcmp(parameters[0], COMMAND_REMOVE) == 0) {

		DIE(tokenIndex != COMMAND_REMOVE_ALLOWED_TOKEN_NUMBER,
		ARGUMENT_NUMBER_MISMATCH_EXCEPTION);

		hash_table_remove(hashTable, parameters[1]);

	} else if (strcmp(parameters[0], COMMAND_FIND) == 0) {

		DIE(tokenIndex > COMMAND_FIND_ALLOWED_TOKEN_NUMBER,
		ARGUMENT_NUMBER_MISMATCH_EXCEPTION);

		DIE(tokenIndex == COMMAND_NO_ARGUMENTS,
		ARGUMENT_NUMBER_MISMATCH_EXCEPTION);

		if (tokenIndex == COMMAND_FIND_ALLOWED_TOKEN_NUMBER)
			hash_table_find(hashTable, parameters[1],
			parameters[2]);
		else
			hash_table_find(hashTable, parameters[1], NULL);

	} else if (strcmp(parameters[0], COMMAND_CLEAR) == 0) {

		DIE(tokenIndex != COMMAND_CLEAR_ALLOWED_TOKEN_NUMBER,
		ARGUMENT_NUMBER_MISMATCH_EXCEPTION);

		hash_table_clear(hashTable);

	} else if (strcmp(parameters[0], COMMAND_PRINT_BUCKET) == 0) {

		DIE(tokenIndex > COMMAND_PRINT_BUCKET_ALLOWED_TOKEN_NUMBER,
		ARGUMENT_NUMBER_MISMATCH_EXCEPTION);

		DIE(tokenIndex == COMMAND_NO_ARGUMENTS,
		ARGUMENT_NUMBER_MISMATCH_EXCEPTION);

		bucketIndex = strtol(parameters[1], NULL, BASE_TEN);

		if (strcmp(parameters[1], ZERO) != 0)
			DIE(bucketIndex == 0,
			BUCKET_INDEX_INVALID_INTEGER_EXCEPTION);

		if (tokenIndex == COMMAND_PRINT_BUCKET_ALLOWED_TOKEN_NUMBER)
			hash_table_print_bucket(hashTable, bucketIndex,
			parameters[2]);
		else
			hash_table_print_bucket(hashTable, bucketIndex, NULL);

	} else if (strcmp(parameters[0], COMMAND_PRINT) == 0) {
		DIE(tokenIndex > COMMAND_PRINT_ALLOWED_TOKEN_NUMBER,
		ARGUMENT_NUMBER_MISMATCH_EXCEPTION);

		if (tokenIndex == COMMAND_PRINT_ALLOWED_TOKEN_NUMBER)
			hash_table_print(hashTable, parameters[1]);
		else
			hash_table_print(hashTable, NULL);

	} else if (strcmp(parameters[0], COMMAND_RESIZE) == 0) {
		DIE(tokenIndex != COMMAND_RESIZE_ALLOWED_TOKEN_NUMBER,
		ARGUMENT_NUMBER_MISMATCH_EXCEPTION);

		if (strcmp(parameters[1], COMMAND_RESIZE_DOUBLE) == 0)
			hash_table_resize(hashTable, 0);
		else if (strcmp(parameters[1], COMMAND_RESIZE_HALVE) == 0)
			hash_table_resize(hashTable, 1);
		else
			DIE(ALWAYS_TRUE,
			RESIZE_OPERATION_NOT_SUPPORTED_EXCEPTION);

	} else {
		DIE(ALWAYS_TRUE, INVAlID_OPERATION_EXCEPTION);
	}

	free(separator);

	for (i = 0; i < FUNCTION_PARAMETERS; i++)
		free(parameters[i]);

	free(parameters);
}

void stdin_read(HashTable *hashTable)
{
	while (fgets(commandBuffer, MAX_COMMAND_SIZE, stdin))
		parse_command(commandBuffer, hashTable);

}

void file_read(char *fileName, HashTable *hashTable)
{
	FILE *fp;

	fp = fopen(fileName, FILE_READ);
	DIE(fp == NULL, FILE_NOT_FOUND_EXCEPTION);

	while ((fgets(commandBuffer, MAX_COMMAND_SIZE, fp)))
		parse_command(commandBuffer, hashTable);

	fclose(fp);
}
