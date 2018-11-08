#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"

#define BASE_TEN 10

int main (int argc, char **argv)
{
	int i;
	unsigned int initialHashTableSize;
	HashTable hashTable;

	initialHashTableSize = 0;

	if (argc == 1)
		DIE(ALWAYS_TRUE, NOT_ENOUGH_ARGUMENTS_EXCEPTION);

	for (i = 1; i < argc; i++) {
		if (i == 1) {
			initialHashTableSize = strtol(argv[i], NULL, BASE_TEN);
			DIE(initialHashTableSize == 0,
			HASH_TABLE_SIZE_EXCEPTION);
			hash_table_initialize(&hashTable, initialHashTableSize);
			if (argc == 2)
				stdin_read(&hashTable);
		} else {
			file_read(argv[i], &hashTable);
		}
	}

	if (initialHashTableSize != 0)
		hash_table_free(&hashTable);

	return 0;
}
