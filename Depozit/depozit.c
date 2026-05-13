#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

typedef struct Produs Produs;
typedef struct Nod Nod;
typedef struct HashTable HashTable;

struct Produs {
	char* nume;
	int codCategorie;
	int cantitate;
};

struct Nod {
	Produs info;
	Nod* next;
};

struct HashTable {
	int dimensiune;
	Nod** vector;
};

Produs initProdus(const char* nume, int cod, int cantitate) {
	Produs p;
	p.codCategorie = cod;
	p.cantitate = cantitate;
	p.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	if (p.nume) {
		strcpy(p.nume, nume);
	}
	return p;
}

void inserareLaSfarsit(Nod** cap, Produs p) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->next = NULL;
	nou->info = p;
	if (*cap == NULL) {
		*cap = nou;
	}
	else {
		Nod* aux = *cap;
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = nou;
	}
}

HashTable initHashTable(int size) {
	HashTable tabela;
	tabela.dimensiune = size;
	tabela.vector = (Nod**)malloc(sizeof(Nod*) * size);
	for (int i = 0; i < size; i++) {
		tabela.vector[i] = NULL;
	}
	return tabela;
}

int hash(int dim, int cod) {
	return cod % dim;
}

void inserareHashTable(HashTable tabela, Produs p) {
	if (tabela.dimensiune > 0) {
		int pozitie = hash(tabela.dimensiune, p.codCategorie);
		inserareLaSfarsit(&(tabela.vector[pozitie]), p);
	}
}

void afisareHashTable(HashTable tabela) {
	for (int i = 0; i < tabela.dimensiune; i++) {
		if (tabela.vector[i] != NULL) {
			printf("\nPozitie %d: ", i);
			Nod* aux = tabela.vector[i];
			while (aux) {
				printf("\n %s - cat: %d, stoc: %d", aux->info.nume, aux->info.codCategorie, aux->info.cantitate);
				aux = aux->next;
			}
		}
	}
	printf("\n");
}

int calculStocPerCategorie(HashTable tabela, int codCautat) {
	int pozitie = hash(tabela.dimensiune, codCautat);
	int total = 0;
	Nod* aux = tabela.vector[pozitie];
	while (aux) {
		if (aux->info.codCategorie == codCautat) {
			total += aux->info.cantitate;
		}
		aux = aux->next;
	}
	return total;
}

void dezalocareHashTable(HashTable* tabela) {
	for (int i = 0; i < tabela->dimensiune; i++) {
		Nod* aux = tabela->vector[i];
		while (aux) {
			free(aux->info.nume);
			Nod* deSters = aux;
			aux = aux->next;
			free(deSters);
		}
	}
	free(tabela->vector);
	tabela->vector = NULL;
	tabela->dimensiune = 0;
}

int main() {
	HashTable tabela = initHashTable(5);
	FILE* f = fopen("produse.txt", "r");
	if (f) {
		char buffer[100];
		int cod, cantitate;
		while (fscanf(f, "%s %d %d", buffer, &cod, &cantitate) == 3) {
			inserareHashTable(tabela, initProdus(buffer, cod, cantitate));
		}
		fclose(f);
	}
	else {
		printf("nu se deschide fisierul..");
	}

	printf("continut hashtable");
	afisareHashTable(tabela);

	int categorieCautata = 12;
	printf("\nstocul pentru categoria %d este %d", categorieCautata, calculStocPerCategorie(tabela, categorieCautata));

	dezalocareHashTable(&tabela);

	return 0;
}