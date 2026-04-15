#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct Heap {
	int* v;
	int dim;
} Heap;

Heap creareHeap(int dim) {
	Heap h;
	h.dim = dim;
	h.v = (int*)malloc(sizeof(int) * dim);
	return h;
}

void heapify(Heap h, int i) {
	int max = i;
	int st = 2 * 1 + 1;
	int dr = 2 * i + 2;

	if (st < h.dim && h.v[st] > h.v[max]) max = st;
	if (dr < h.dim && h.v[dr] > h.v[max]) max = dr;

	if (max != i) {
		int temp = h.v[i];
		h.v[i] = h.v[max];
		h.v[max] = temp;

		heapify(h, max);
	}
}

void construireHeap(Heap h) {
	for (int i = (h.dim / 2) - 1; i >= 0; i--) {
		heapify(h, i);
	}
}

void afisareHeap(Heap h) {
	for (int i = 0; i < h.dim; i++) {
		printf("%d ", h.v[i]);
	}
	printf("\n");
}

int extragereMax(Heap* h) {
	if (h->dim == 0) return -1;
	int max = h->v[0];
	h->v[0] = h->v[h->dim - 1];
	h->dim--;
	heapify(*h, 0);
	return max;
}

void inserareHeap(Heap* h, int val) {
	h->dim++;
	h->v = (int*)realloc(h->v, sizeof(int) * h->dim);
	int i = h->dim - 1;
	h->v[i] = val;
	while (i != 0 && h->v[(i - 1) / 2] < h->v[i]) {
		int temp = h->v[i];
		h->v[i] = h->v[(i - 1) / 2];
		h->v[(i - 1) / 2] = temp;
		i = (i - 1) / 2;
	}
}

int main() {

	Heap h = creareHeap(5);

	h.v[0] = 15;
	h.v[1] = 27;
	h.v[2] = 29;
	h.v[3] = 36;
	h.v[4] = 85;

	printf("Vector initial:\n");
	afisareHeap(h);

	construireHeap(h);

	printf("Heap:\n");
	afisareHeap(h);

	printf("Max extras: %d\n", extrageMax(&h));

	printf("Dupa extragere:\n");
	afisareHeap(h);

	inserareHeap(&h, 60);

	printf("Dupa inserare 60:\n");
	afisareHeap(h);

	free(h.v);

	return 0;
}