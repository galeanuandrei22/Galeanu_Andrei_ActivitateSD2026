#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Zbor {
	int idZbor;
	int nrPasageri;
	float pretBilet;
	char* destinatie;
	char* pilot;
	unsigned char codPoarta;
};

typedef struct Zbor Zbor;

struct Nod {
	Zbor info;
	struct Nod* st;
	struct Nod* dr;
};

typedef struct Nod Nod;

void afisareZbor(Zbor z) {
	printf("zbor: %d - %s - pilot: %s - pasageri: %d - poarta: %c\n", z.idZbor, z.destinatie, z.pilot, z.nrPasageri, z.codPoarta);
}

Zbor initZbor(int id, int nrP, float pret, const char* dest, const char* pilot, unsigned char poarta) {
	Zbor z;
	z.idZbor = id;
	z.nrPasageri = nrP;
	z.pretBilet = pret;
	z.codPoarta = poarta;
	z.destinatie = (char*)malloc(strlen(dest) + 1);
	strcpy(z.destinatie, dest);
	z.pilot = (char*)malloc(strlen(pilot) + 1);
	strcpy(z.pilot, pilot);
	return z;
}

int maxim(int a, int b) {
	return (a > b ? a : b);
}

int calculeazaInaltime(Nod* rad) {
	if (rad) {
		return maxim(calculeazaInaltime(rad->st), calculeazaInaltime(rad->dr)) + 1;
	}
	return 0;
}

int calculDiferentaInaltimi(Nod* rad) {
	if (rad != NULL) {
		return calculeazaInaltime(rad->st) - calculeazaInaltime(rad->dr);
	}
	return 0;
}

void rotireLaStanga(Nod** rad) {
	Nod* aux = (*rad)->dr;
	(*rad)->dr = aux->st;
	aux->st = (*rad);
	(*rad) = aux;
}

void rotireLaDreapta(Nod** rad) {
	Nod* aux = (*rad)->st;
	(*rad)->st = aux->dr;
	aux->dr = (*rad);
	(*rad) = aux;
}

void adaugaZborInAVL(Nod** rad, Zbor z) {
	if (*rad == NULL) {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = z;
		nou->st = nou->dr = NULL;
		*rad = nou;
	}
	else {
		if (z.idZbor < (*rad)->info.idZbor) adaugaZborInAVL(&((*rad)->st), z);
		else if (z.idZbor > (*rad)->info.idZbor) adaugaZborInAVL(&((*rad)->dr), z);
	}

	int dif = calculDiferentaInaltimi(*rad);
	if (dif == 2) {
		if (calculDiferentaInaltimi((*rad)->st) == -1) {
			rotireLaStanga(&((*rad)->st));
		}
		rotireLaDreapta(rad);
	}
	if (dif == -2) {
		if (calculDiferentaInaltimi((*rad)->dr) == 1) {
			rotireLaDreapta(&((*rad)->dr));
		}
		rotireLaStanga(rad);
	}
}

int totalPasageriDestinatie(Nod* rad, const char* dest) {
	if (rad) {
		int suma = 0;
		if (strcmp(rad->info.destinatie, dest) == 0) {
			suma = rad->info.nrPasageri;
		}
		return suma + totalPasageriDestinatie(rad->st, dest) + totalPasageriDestinatie(rad->dr, dest);
	}
	return 0;
}

void afisarePreOrdine(Nod* rad) {
	if (rad) {
		afisareZbor(rad->info);
		afisarePreOrdine(rad->st);
		afisarePreOrdine(rad->dr);
	}
}

void dezalocare(Nod** rad) {
	if (*rad) {
		dezalocare(&((*rad)->st));
		dezalocare(&((*rad)->dr));
		free((*rad)->info.destinatie);
		free((*rad)->info.pilot);
		free(*rad);
		*rad = NULL;
	}
}

int main() {
	Nod* rad = NULL;
	FILE* f = fopen("zboruri.txt", "r");
	if (f) {
		char buffer[150], sep[] = ",\n";
		while (fgets(buffer, 150, f)) {
			char* aux = strtok(buffer, sep);
			int id = atoi(aux);
			int nrP = atoi(strtok(NULL, sep));
			float pret = (float)atof(strtok(NULL, sep));
			char* dest = strtok(NULL, sep);
			char* pilot = strtok(NULL, sep);
			unsigned char poarta = *strtok(NULL, sep);
			adaugaZborInAVL(&rad, initZbor(id, nrP, pret, dest, pilot, poarta));
		}
		fclose(f);
	}
	printf("zboruri preordine\n");
	afisarePreOrdine(rad);

	char deCautat[] = "Paris";
	printf("\nnr total de pasageri catre %s: %d", deCautat, totalPasageriDestinatie(rad, deCautat));
	printf("\ninaltime arbore: %d", calculeazaInaltime(rad));
	dezalocare(&rad);

	return 0;
}