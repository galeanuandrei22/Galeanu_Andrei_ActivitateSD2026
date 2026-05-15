#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Proces {
	int idProces;
	int nrThreaduri;
	float cuantaTimp;
	char* numeProces;
	char* utilizator;
	unsigned char stare;
};

typedef struct Proces Proces;

struct Nod {
	Proces info;
	struct Nod* st;
	struct Nod* dr;
};

typedef struct Nod Nod;

void afisareProces(Proces p) {
	printf("id: %d - %s, utilizator: %s, threads: %d, cuanta: %.2f, stare: %c\n", p.idProces, p.numeProces, p.utilizator, p.nrThreaduri, p.cuantaTimp, p.stare);
}

Proces initProces(int id, int nrT, float cuanta, const char* nume, const char* util, unsigned char stare) {
	Proces p;
	p.idProces = id;
	p.nrThreaduri = nrT;
	p.cuantaTimp = cuanta;
	p.stare = stare;

	p.numeProces = (char*)malloc(strlen(nume) + 1);
	strcpy(p.numeProces, nume);

	p.utilizator = (char*)malloc(strlen(util) + 1);
	strcpy(p.utilizator, util);

	return p;
}

int maxim(int a, int b) {
	return (a > b ? a : b);
}

int calculeazaInaltimeArbore(Nod* rad) {
	if (rad) {
		return maxim(calculeazaInaltimeArbore(rad->st), calculeazaInaltimeArbore(rad->dr)) + 1;
	}
	return 0;
}

int calculDiferentaInaltimi(Nod* rad) {
	if (rad != NULL) {
		return calculeazaInaltimeArbore(rad->st) - calculeazaInaltimeArbore(rad->dr);
	}
	else {
		return 0;
	}
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

void adaugaProcesInArbore(Nod** rad, Proces procesNou) {
	if (*rad == NULL) {
		Nod* nod = malloc(sizeof(Nod));
		nod->info = procesNou;
		nod->dr = NULL;
		nod->st = NULL;
		*rad = nod;
	}
	else {
		if ((*rad)->info.idProces > procesNou.idProces) {
			adaugaProcesInArbore(&((*rad)->st), procesNou);
		}
		if ((*rad)->info.idProces < procesNou.idProces) {
			adaugaProcesInArbore(&((*rad)->dr), procesNou);
		}
	}

	int diferentaInaltimi = calculDiferentaInaltimi(*rad);
	if (diferentaInaltimi == 2) {
		if (calculDiferentaInaltimi((*rad)->st) == -1) {
			rotireLaStanga(&(*rad)->st);
		}
		rotireLaDreapta(rad);
	}
	if (diferentaInaltimi == -2) {
		if (calculDiferentaInaltimi((*rad)->dr) == -1) {
			rotireLaDreapta(&(*rad)->dr);
		}
		rotireLaStanga(rad);
	}
}

void afisarePreordine(Nod* rad) {
	if (rad) {
		afisareProces(rad->info);
		afisarePreordine(rad->st);
		afisarePreordine(rad->dr);
	}
}

int calculeazaThreaduriUtilizator(Nod* rad, const char* util) {
	if (rad) {
		int threaduriCurent = 0;
		if (strcmp(rad->info.utilizator, util) == 0) {
			threaduriCurent = rad->info.nrThreaduri;
		}
		return threaduriCurent + calculeazaThreaduriUtilizator(rad->st, util) + calculeazaThreaduriUtilizator(rad->dr, util);
	}
	return 0;
}

void dezalocareArbore(Nod** rad) {
	if (*rad) {
		dezalocareArbore(&(*rad)->st);
		dezalocareArbore(&(*rad)->dr);
		free((*rad)->info.numeProces);
		free((*rad)->info.utilizator);
		free(*rad);
		*rad = NULL;
	}
}

int main() {
	Nod* rad = NULL;
	FILE* f = fopen("procese.txt", "r");

	if (f) {
		char buffer[150];
		char sep[3] = ",\n";

		while (fgets(buffer, 150, f)) {
			char* aux = strtok(buffer, sep);
			int id = atoi(aux);
			int nrT = atoi(strtok(NULL, sep));
			float cuanta = (float)atof(strtok(NULL, sep));

			char* nume = strtok(NULL, sep);
			char* util = strtok(NULL, sep);
			unsigned char stare = *strtok(NULL, sep);

			Proces p = initProces(id, nrT, cuanta, nume, util, stare);
			adaugaProcesInArbore(&rad, p);
		}
		fclose(f);
	}
	else {
		printf("nu s-a reusit deschiderea fisierului!\n");
		return 1;
	}
	printf("afisare preordine\n");
	afisarePreordine(rad);
	printf("inaltime arbore: %d\n", calculeazaInaltimeArbore(rad));

	const char userCautat = "root";
	printf("nr total de thread-uri pt userul %s: %d\n", userCautat, calculeazaThreaduriUtilizator(rad, userCautat));
	dezalocareArbore(&rad);
	return 0;
}