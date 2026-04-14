#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Student {
	int id;
	char* nume;
	float medie;
} Student;

typedef struct Nod {
	Student info;
	struct Nod* next;
} Nod;

typedef struct HashTable {
	int dim;
	Nod** tabela;
} HashTable;

int functieHash(int cheie, int dim) {
	return cheie % dim;
}

Student creareStudent(int id, const char* nume, float medie) {
	Student s;
	s.id = id;
	s.nume = (char*)malloc(strlen(nume) + 1);
	strcpy(s.nume, nume);
	s.medie = medie;

	return s;
}

HashTable initializare(int dim) {
	HashTable ht;
	ht.dim = dim;
	ht.tabela = (Nod**)malloc(sizeof(Nod*) * dim);

	for (int i = 0; i < dim; i++) ht.tabela[i] = NULL;

	return ht;
}

void inserare(HashTable ht, Student s) {
	int poz = functieHash(s.id, ht.dim);

	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = s;
	nou->next = ht.tabela[poz];

	ht.tabela[poz] = nou;
}

void afisare(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		printf("Pozitia %d: \n", i);
		Nod* p = ht.tabela[i];
		while (p) {
			printf("ID: %d, Nume: %s, Medie: %.2f\n", p->info.id, p->info.nume, p->info.medie);
			p = p->next;
		}
		printf("\n");
	}
}

Student* cautare(HashTable ht, int id) {
	int poz = functieHash(id, ht.dim);
	Nod* p = ht.tabela[poz];
	while (p) {
		if (p->info.id == id) return &p->info;
		p = p->next;
	}
	return NULL;
}

void dezalocare(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		Nod* p = ht.tabela[i];
		while (p) {
			free(p->info.nume);
			Nod* temp = p;
			p = p->next;
			free(temp);
		}
	}
	free(ht.tabela);
}

int main() {

	HashTable ht = initializare(5);

	inserare(ht, creareStudent(1, "Ana", 9.5));
	inserare(ht, creareStudent(6, "Ion", 8.2));
	inserare(ht, creareStudent(11, "Maria", 7.8));
	inserare(ht, creareStudent(3, "George", 6.5));

	printf("HashTable:\n");
	afisare(ht);

	Student* s = cautare(ht, 6);

	if (s)
		printf("\nGasit: %s\n", s->nume);

	dezalocare(ht);

	return 0;
}