#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct Nod {
	int valoare;
	struct Nod* stanga;
	struct Nod* dreapta
} Nod;

Nod* createNod(int val) {
	Nod* nod = (Nod*)malloc(sizeof(Nod));

	nod->valoare = val;
	nod->stanga = NULL;
	nod->dreapta = NULL;

	return nod;
}

Nod* inserare(Nod* radacina, int val) {
	if (radacina == NULL) return createNod(val);
	if (val < radacina->valoare) radacina->stanga = inserare(radacina->stanga, val);
	else radacina->dreapta = inserare(radacina->dreapta, val);
	return radacina;
}

void inordine(Nod* rad) {
	if (rad) {
		inordine(rad->stanga);
		printf("%d ", rad->valoare);
		inordine(rad->dreapta);
	}
}

void preordine(Nod* rad) {
	if (rad) {
		printf("%d ", rad->valoare);
		preordine(rad->stanga);
		preordine(rad->dreapta);
	}
}

	void postordine(Nod* rad) {

	if (rad) {
		postordine(rad->stanga);
		postordine(rad->dreapta);
		printf("%d ", rad->valoare);
	}
}

void postordine(Nod* rad) {

	if (rad) {
		postordine(rad->stanga);
		postordine(rad->dreapta);
		printf("%d ", rad->valoare);
	}
}

int cautare(Nod* rad, int val) {
	if (rad == NULL) return 0;
	if (rad->valoare == val) return 1;
	if (val < rad->valoare) return cautare(rad->stanga, val);
	else return cautare(rad->dreapta, val);
}

int maxim(Nod* rad) {
	while (rad->dreapta) rad = rad->dreapta;
	return rad->valoare;
}

int inaltime(Nod* rad) {
	if (rad == NULL) return 0;

	int st = inaltime(rad->stanga);
	int dr = inaltime(rad->dreapta);

	return 1 + (st > dr ? st : dr);
}

void dezalocare(Nod* rad) {
	if (rad) {
		dezalocare(rad->stanga);
		dezalocare(rad->dreapta);
		free(rad);
	}
}

int main() {

	Nod* rad = NULL;
	rad = inserare(rad, 10);
	rad = inserare(rad, 5);
	rad = inserare(rad, 15);
	rad = inserare(rad, 2);
	rad = inserare(rad, 7);

	printf("Inordine: ");
	inordine(rad);

	printf("\nPreordine: ");
	preordine(rad);

	printf("\nPostordine: ");
	postordine(rad);

	printf("\n\nCautare 7: %d", cautare(rad, 7));

	printf("\nMaxim: %d", maxim(rad));

	printf("\nInaltime: %d", inaltime(rad));

	dezalocare(rad);

	return 0;
}
