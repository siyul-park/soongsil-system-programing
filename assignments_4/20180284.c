#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "20180284.h"

#define MAX_STR_SIZE 256

typedef struct Chunk {
	char *name;
    size_t len;
	struct Chunk *next;
} Chunk;

typedef struct Chunks {
	Chunk *first;
} Chunks;


typedef struct Memory {
	char *heep;

    size_t len;
	char *next;

	Chunks chunks;
} Memory;

void dump_heap(const void *mem, size_t len) {
	const char *buffer = mem;
	size_t i;
	for (i = 0; i < len; i++) {
	    if (i > 0 && i % 16 == 0) {
            printf("\n");
		}
        printf("%02x ", buffer[i] & 0xff);
    }
	puts(""); 
}

void dump_mem(const Memory *mem) {
	dump_heap(mem->heep, mem->len);
	printf("\n");
}

Memory create_mem(size_t len) {
	char *heep = calloc(sizeof(char), len);
	char *next = heep;
	Chunks chunks = { .first = 0 };

	return (Memory) { .chunks = chunks, .heep = heep, .len = len, .next = next };
}

void free_mem(Memory *mem) {
	free(mem->heep);
	
	Chunk *currnet = mem->chunks.first;
	while (currnet != 0) {
		Chunk *be_free = currnet;
		currnet = currnet->next;
		free(be_free);
	}
}

int size_of_free_mem(const Memory *mem) {
	return mem->len - (mem->next - mem->heep);
}

bool can_allocate_mem(const Memory *mem, size_t value_size) {
	return value_size <= size_of_free_mem(mem);
}

void allocate_mem(Memory *mem, const char *name, const void *value, size_t value_size) {
	memcpy(mem->next, value, value_size);
	mem->next += value_size;

	Chunk *last = mem->chunks.first;
	while (last != 0 && last->next != 0) {
		last = last->next;
	}

	if (last == 0) {
		Chunk *chunk = malloc(sizeof(Chunk));
		char *name_cpy = calloc(sizeof(char), strlen(name) + 1);
		strcpy(name_cpy, name);

		chunk->len = value_size;
		chunk->name = name_cpy;
		chunk->next = 0;

		mem->chunks.first = chunk;
	} else {
		if (strcmp(last->name, name) == 0) {
			last->len += value_size;
		} else {
			Chunk *chunk = malloc(sizeof(Chunk));
			char *name_cpy = calloc(sizeof(char), strlen(name) + 1);
			strcpy(name_cpy, name);

			chunk->len = value_size;
			chunk->name = name_cpy;
			chunk->next = 0;

			last->next = chunk;
		}
	}
}

void deallocate_mem(Memory *mem, char *name) {
	Chunk *pre = 0;
	Chunk *currnet = mem->chunks.first;

	char *location = mem->heep;
	while (currnet != 0 && strcmp(currnet->name, name) != 0) {
		location += currnet->len;

		pre = currnet;
		currnet = currnet->next;
	}
	
	if (currnet == 0) {
		return;
	}

	for (char *ch = location; ch < mem->next; ch++) {
		char *next = ch + currnet->len;

		char value = 0;
		if (next < mem->next) {
			value = *next;
		}

		*ch = value;
	}

	mem->next -= currnet->len;

	if (pre == 0) {
		mem->chunks.first = currnet->next;
	} else {
		pre->next = currnet->next;
	}

	free(currnet->name);
	free(currnet);
}

int main() {
	Memory mem = create_mem(256);

	while (true) {
		printf("Do you want to allocate data (1) or deallocate (2) ?\n");
		
		int operator = 0;
		scanf("%d", &operator);
		assert(operator == 1 || operator == 2);

		// allocate 
		if (operator == 1) {
			printf("What is the type of data want to allocate and what will be the name of the data?\n");
			
			char type[MAX_STR_SIZE] = { 0 };
			char name[MAX_STR_SIZE] = { 0 };
			scanf("%s %s", type, name);

			if (strcmp(type, "int") == 0) {
				size_t value_size = sizeof(int);
				if (!can_allocate_mem(&mem, value_size)) {
					printf("There is not enough memory for the data, you can only use %d byte(s)\n", size_of_free_mem(&mem));
					break;
				}

				printf("Please specify a value for the data type\n");
				int value = 0;
				scanf("%d", &value);

				allocate_mem(&mem, name, &value, value_size);

				printf("%s has been allocated successfully\n", name);
				printf("Here is the mem dump\n");
				dump_mem(&mem);
			} else if (strcmp(type, "double") == 0) {
				size_t value_size = sizeof(double);
				if (!can_allocate_mem(&mem, value_size)) {
					printf("There is not enough memory for the data, you can only use %d byte(s)\n", size_of_free_mem(&mem));
					break;
				}

				printf("Please specify a value for the data type\n");
				double value = 0.0;
				scanf("%lf", &value);

				allocate_mem(&mem, name, &value, value_size);

				printf("%s has been allocated successfully\n", name);
				printf("Here is the mem dump\n");
				dump_mem(&mem);
			} else if (strcmp(type, "char") == 0) {
				size_t value_size = sizeof(char);
				if (!can_allocate_mem(&mem, value_size)) {
					printf("There is not enough memory for the data, you can only use %d byte(s)\n", size_of_free_mem(&mem));
					break;
				}

				printf("Please specify a value for the data type\n");
				char value = 0.0;
				scanf("%c", &value);

				allocate_mem(&mem, name, &value, value_size);

				printf("%s has been allocated successfully\n", name);
				printf("Here is the mem dump\n");
				dump_mem(&mem);
			} else if (strcmp(type, "struct") == 0) {
				printf("Please specify a number of value for the data type\n");
				int count = 0;
				scanf("%d", &count);

				printf("Please specify data types and a values for the data type\n");
				for (int i = 0; i < count; i++) {					
					char type[MAX_STR_SIZE] = { 0 };
					scanf("%s ", type);

					if (strcmp(type, "int") == 0) {
						size_t value_size = sizeof(int);
						if (!can_allocate_mem(&mem, value_size)) {
							printf("There is not enough memory for the data, you can only use %d byte(s)\n", size_of_free_mem(&mem));
							break;
						}

						int value = 0;
						scanf("%d", &value);

						allocate_mem(&mem, name, &value, value_size);	
					} else if (strcmp(type, "double") == 0) {
						size_t value_size = sizeof(double);
						if (!can_allocate_mem(&mem, value_size)) {
							printf("There is not enough memory for the data, you can only use %d byte(s)\n", size_of_free_mem(&mem));
							break;
						}

						double value = 0;
						scanf("%lf", &value);

						allocate_mem(&mem, name, &value, value_size);	
					} else if (strcmp(type, "char") == 0) {
						size_t value_size = sizeof(char);
						if (!can_allocate_mem(&mem, value_size)) {
							printf("There is not enough memory for the data, you can only use %d byte(s)\n", size_of_free_mem(&mem));
							break;
						}

						char value = 0;
						scanf("%c", &value);

						allocate_mem(&mem, name, &value, value_size);	
					}
				}

				printf("%s has been allocated successfully\n", name);
				printf("Here is the mem dump\n");
				dump_mem(&mem);
			} else {
				printf("%s is not supported\n", type);
			}
		// deallocate
		} else {
			printf("What is the name of data want to deallocate?\n");
			
			char name[MAX_STR_SIZE] = { 0 };
			scanf("%s", name);

			deallocate_mem(&mem, name);

			printf("%s has been deallocated successfully\n", name);
			printf("Here is the mem dump\n");
			dump_mem(&mem);
		}
	}
	
	free_mem(&mem);
	return 0;
}