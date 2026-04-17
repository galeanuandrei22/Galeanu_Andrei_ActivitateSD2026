#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TABLE_SIZE 100
#define MAX_WORD_LEN 30

typedef struct Node {
	char word[MAX_WORD_LEN];
	int count;
	struct Node* next;
} Node;

typedef struct HashTable {
	Node* buckets[TABLE_SIZE];
} HashTable;

unsigned int hash(char* word) {
	unsigned int hash_val = 0;
	for (int i = 0; word[i] != '\0'; i++) {
		hash_val = hash_val * 31 + tolower(word[i]);
	}
	return hash_val % TABLE_SIZE;
}

HashTable* createTable() {
	HashTable* table = malloc(sizeof(HashTable));
	for (int i = 0; i < TABLE_SIZE; i++) {
		table->buckets[i] = NULL;
	}
	return table;
}

void insertWord(HashTable* table, char* word) {
	unsigned int index = hash(word);
	Node* current = table->buckets[index];
	while (current != NULL) {
		if (_stricmp(current->word, word) == 0) {
			current->count++;
			return;
		}
		current = current->next;
	}
	Node* newNode = malloc(sizeof(Node));
	strncpy(newNode->word, word, MAX_WORD_LEN);
	newNode->count = 1;
	newNode->next = table->buckets[index];
	table->buckets[index] = newNode;
}

void displayFrequencies(HashTable* table) {
	printf("\nFrecventa cuvintelor\n");
	for (int i = 0; i < TABLE_SIZE; i++) {
		Node* current = table->buckets[i];
		while (current != NULL) {
			printf("%s: %d\n", current->word, current->count);
			current = current->next;
		}
	}
}

void freeTable(HashTable* table) {
	for (int i = 0; i < TABLE_SIZE; i++) {
		Node* current = table->buckets[i];
		while (current != NULL) {
			Node* temp = current;
			current = current->next;
			free(temp);
		}
	}
	free(table);
}

int main() {
	HashTable* myTable = createTable();
	char text[] = "Ana are mere. Cate mere are Ana?";
	printf("text analizat: %s\n", text);
	char* token = strtok(text, " ,.!;?");
	while (token != NULL) {
		insertWord(myTable, token);
		token = strtok(NULL, " ,.!;?");
	}
	displayFrequencies(myTable);
	freeTable(myTable);
	return 0;
}