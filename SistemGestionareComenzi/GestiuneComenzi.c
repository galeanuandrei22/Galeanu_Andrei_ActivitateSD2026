#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Comanda {
	int id;
	int timpLivrare;
	float greutate;
	char* destinatar;
	char* adresa;
	unsigned char prioritate;
};
typedef struct Comanda Comanda;

struct Heap {
	int lungime;
	Comanda* elemente;
	int nrElemente;
};
typedef struct Heap Heap;

Comanda citireComandaDinFisier(FILE* file) {
	char buffer[200];
	char sep[3] = ",\n";
	if (fgets(buffer, 200, file) == NULL) {
		Comanda goala = { 0 };
		return goala;
	}
	char* aux;
	Comanda c;

	aux = strtok(buffer, sep);
	c.id = atoi(aux);
	c.timpLivrare = atoi(strtok(NULL, sep));
	c.greutate = (float)atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	c.destinatar = (char*)malloc(strlen(aux) + 1);
	strcpy(c.destinatar, aux);

	aux = strtok(NULL, sep);
	c.adresa = (char*)malloc(strlen(aux) + 1);
	strcpy(c.adresa, aux);

	c.prioritate = *strtok(NULL, sep);

	return c;
}

void afisareComanda(Comanda c) {
	printf("<%d> livrare in: %dmin - destinatar: %s - prioritate: %c\n", c.id, c.timpLivrare, c.destinatar, c.prioritate);
}

Heap initializareHeap(int lungime) {
	Heap h;
	h.lungime = lungime;
	h.elemente = (Comanda*)malloc(sizeof(Comanda) * lungime);
	h.nrElemente = 0;
	return h;
}

void filtreazaHeap(Heap heap, int poz) {
	int pozSt = 2 * poz + 1;
	int pozDr = 2 * poz + 2;
	int pozMin = poz;

	if (pozSt < heap.nrElemente && heap.elemente[pozMin].timpLivrare > heap.elemente[pozSt].timpLivrare)
		pozMin = pozSt;
	if (pozDr < heap.nrElemente && heap.elemente[pozMin].timpLivrare > heap.elemente[pozDr].timpLivrare)
		pozMin = pozDr;

	if (pozMin != poz) {
		Comanda aux = heap.elemente[pozMin];
		heap.elemente[pozMin] = heap.elemente[poz];
		heap.elemente[poz] = aux;
		if (pozMin < (heap.nrElemente - 2) / 2) {
			filtreazaHeap(heap, pozMin);
		}
	}
}

Comanda extrageComanda(Heap* h) {
	Comanda c;
	c.id = -1;
	if (h->nrElemente > 0) {
		c = h->elemente[0];
		h->elemente[0] = h->elemente[h->nrElemente - 1];
		h->elemente[h->nrElemente - 1] = c;
		h->nrElemente--;
		for (int i = (h->nrElemente - 2) / 2; i >= 0; i--) {
			filtreazaHeap(*h, i);
		}
	}
	return c;
}

void dezalocareHeap(Heap* h) {
	for (int i = 0; i < h->lungime; i++) {
		free(h->elemente[i].destinatar);
		free(h->elemente[i].adresa);
	}
	free(h->elemente);
	h->elemente = NULL;
}

int main() {
	Heap heap = initializareHeap(10);
	FILE* f = fopen("comenzi.txt", "r");
	if (f) {
		while (!feof(f)) {
			Comanda c = citireComandaDinFisier(f);
			if (c.id != 0) heap.elemente[heap.nrElemente++] = c;
		}
		fclose(f);
	}

	for (int i = (heap.nrElemente - 2) / 2; i >= 0; i--) {
		filtreazaHeap(heap, i);
	}

	printf("coada initial\n");
	for (int i = 0; i < heap.nrElemente; i++) {
		afisareComanda(heap.elemente[i]);
	}

	printf("se extrag primele doua comenzi urgente..\n");
	afisareComanda(extrageComanda(&heap));
	afisareComanda(extrageComanda(&heap));

	printf("heap ramas vizibil: \n");
	for (int i = 0; i < heap.nrElemente; i++) {
		afisareComanda(heap.elemente[i]);
	}

	dezalocareHeap(&heap);
	return 0;
}