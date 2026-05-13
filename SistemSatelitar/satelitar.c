#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Senzor {
	int idSenzor;
	int consumEnergie;
	float valoareMasurata;
	char* tipSenzor;
	char* numeUnitate;
	unsigned char stare;
};

typedef struct Senzor Senzor;

struct Nod {
	Senzor info;
	struct Nod* st;
	struct Nod* dr;
};

typedef struct Nod Nod;

Senzor citireSenzorDinFisier(FILE* file) {
	char buffer[150];
	char sep[3] = ",\n";
	if (fgets(buffer, 150, file) == NULL) {
		Senzor s = { -1 };
		return s;
	}

	char* aux;
	Senzor s1;

	aux = strtok(buffer, sep);
	s1.idSenzor = atoi(aux);
	s1.consumEnergie = atoi(strtok(NULL, sep));
	s1.valoareMasurata = (float)atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	s1.tipSenzor = (char*)malloc(strlen(aux) + 1);
	strcpy(s1.tipSenzor, aux);

	aux = strtok(NULL, sep);
	s1.numeUnitate = (char*)malloc(strlen(aux) + 1);
	strcpy(s1.numeUnitate, aux);

	s1.stare = *strtok(NULL, sep);

	return s1;
}

void afisareSenzor(Senzor s) {
	printf("id: %d - tip: %s - consum: %d - valoare: %.2f - unitate: %s - stare: %c\n", s.idSenzor, s.tipSenzor, s.consumEnergie, s.valoareMasurata, s.numeUnitate, s.stare);
}

void adaugaSenzorInArbore(Nod** rad, Senzor sNou) {
	if (*rad == NULL) {
		Nod* nod = (Nod*)malloc(sizeof(Nod));
		nod->info = sNou;
		nod->st = nod->dr = NULL;
		*rad = nod;
	}
	else {
		if (sNou.idSenzor < (*rad)->info.idSenzor) adaugaSenzorInArbore(&((*rad)->st), sNou);
		else if (sNou.idSenzor > (*rad)->info.idSenzor) adaugaSenzorInArbore(&((*rad)->dr), sNou);
	}
}

void afisareInOrdine(Nod* rad) {
	if (rad) {
		afisareInOrdine(rad->st);
		afisareSenzor(rad->info);
		afisareInOrdine(rad->dr);
	}
}

int calculeazaConsumDupaTip(Nod* rad, const char* tipCautat) {
	if (rad) {
		int suma = calculeazaConsumDupaTip(rad->st, tipCautat) + calculeazaConsumDupaTip(rad->dr, tipCautat);
		if (strcmp(rad->info.tipSenzor, tipCautat) == 0) {
			suma += rad->info.consumEnergie;
		}
		return suma;
	}
	return 0;
}

void sumaSiNrSenzoriActivi(Nod* rad, float* suma, int* nr) {
	if (rad) {
		if (rad->info.stare == 'A') {
			(*suma) += rad->info.valoareMasurata;
			(*nr)++;
		}
		sumaSiNrSenzoriActivi(rad->st, suma, nr);
		sumaSiNrSenzoriActivi(rad->dr, suma, nr);
	}
}

Nod* gasesteMaxim(Nod* rad) {
	if (rad == NULL) return NULL;
	while (rad->dr != NULL) rad = rad->dr;
	return rad;
}

void dezalocareArbore(Nod** rad) {
	if (*rad) {
		dezalocareArbore(&((*rad)->st));
		dezalocareArbore(&((*rad)->dr));
		free((*rad)->info.tipSenzor);
		free((*rad)->info.numeUnitate);
		free(*rad);
		*rad = NULL;
	}
}

int main() {
	Nod* rad = NULL;
	FILE* f = fopen("senzori.txt", "r");
	if (f) {
		while (!feof(f)) {
			Senzor s = citireSenzorDinFisier(f);
			if (s.idSenzor != -1) adaugaSenzorInArbore(&rad, s);
		}
		fclose(f);
	}
	printf("senzori satelitari dupa ID\n");
	afisareInOrdine(rad);
	printf("consum total senz termici: %d", calculeazaConsumDupaTip(rad, "Termic"));

	float sumaActivi = 0;
	int nrActivi = 0;
	sumaSiNrSenzoriActivi(rad, &sumaActivi, &nrActivi);
	if (nrActivi > 0) printf("\nvalorea medie senzori activi: %.2f", sumaActivi / nrActivi);

	Nod* maxNod = gasesteMaxim(rad);
	if (maxNod) printf("\nsenz cu id-ul max este: %d - %s", maxNod->info.idSenzor, maxNod->info.tipSenzor);
	printf("\n");
	dezalocareArbore(&rad);
	return 0;
}