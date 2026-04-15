#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct Graf {
	int nrNoduri;
	int** matrice;
} Graf;

Graf creareGraf(int n) {
	Graf g;
	g.nrNoduri = n;
	g.matrice = (int**)malloc(sizeof(int*) * n);
	for (int i = 0; i < n; i++) {
		g.matrice[i] = (int*)malloc(sizeof(int) * n);
		for (int j = 0; j < n; j++) {
			g.matrice[i][j] = 0;
		}
	}
	return g;
}

void adaugaMuchie(Graf g, int i, int j) {
	g.matrice[i][j] = 1;
	g.matrice[j][i] = 1;
}

void afisareGraf(Graf g) {
	for (int i = 0; i < g.nrNoduri; i++) {
		for (int j = 0; j < g.nrNoduri; j++) {
			printf("%d ", g.matrice[i][j]);
		}
		printf("\n");
	}
}

void DFS(Graf g, int start, int* vizitat) {
	printf("%d ", start);
	vizitat[start] = 1;
	for (int i = 0; i < g.nrNoduri; i++) {
		if (g.matrice[start][i] == 1 && !vizitat[i]) {
			DFS(g, i, vizitat);
		}
	}
}

void BFS(Graf g, int start) {
	int* vizitat = (int*)calloc(g.nrNoduri, sizeof(int));
	int* coada = (int*)malloc(sizeof(int) * g.nrNoduri);
	int prim = 0, ultim = 0;
	coada[ultim++] = start;
	vizitat[start] = 1;
	while (prim < ultim) {
		int nod = coada[prim++];
		printf("%d ", nod);
		for (int i = 0; i < g.nrNoduri; i++) {
			if (g.matrice[nod][i] == 1 && !vizitat[i]) {
				coada[ultim++] = i;
				vizitat[i] = 1;
			}
		}
	}
	free(vizitat);
	free(coada);
}

void dezalocareGraf(Graf* g) {
	for (int i = 0; i < g->nrNoduri; i++) {
		free(g->matrice[i]);
	}
	free(g->matrice);
}

int main() {

	Graf g = creareGraf(5);

	adaugaMuchie(g, 0, 1);
	adaugaMuchie(g, 0, 3);
	adaugaMuchie(g, 1, 2);
	adaugaMuchie(g, 1, 3);
	adaugaMuchie(g, 3, 4);

	printf("Matrice de adiacenta:\n");
	afisareGraf(g);

	int* vizitat = (int*)calloc(g.nrNoduri, sizeof(int));

	printf("\nDFS: ");
	DFS(g, 0, vizitat);

	printf("\nBFS: ");
	BFS(g, 0);

	free(vizitat);
	dezalocareGraf(&g);

	return 0;
}