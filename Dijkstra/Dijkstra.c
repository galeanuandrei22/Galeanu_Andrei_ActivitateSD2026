#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <limits.h>

#define N 5

int minDist(int dist[], int vizitat[]) {
	int min = INT_MAX, index = -1;
	for (int i = 0; i < N; i++) {
		if (!vizitat[i] && dist[i] < min) {
			min = dist[i];
			index = i;
		}
	}
	return index;
}

void dijkstra(int graf[N][N], int start) {
	int dist[N];
	int vizitat[N];

	for (int i = 0; i < N; i++) {
		dist[i] = INT_MAX;
		vizitat[i] = 0;
	}

	dist[start] = 0;

	for (int i = 0; i < N - 1; i++) {
		int u = minDist(dist, vizitat);
		vizitat[u] = 1;
		for (int v = 0; v < N; v++) {
			if(!vizitat[v] && graf[u][v] && dist[u] != INT_MAX && dist[u] + graf[u][v] < dist[v]) {
				dist[v] = dist[u] + graf[u][v];
			}
		}
	}
	for (int i = 0; i < N; i++) {
		printf("0 -> %d = %d\n", i, dist[i]);
	}
}

int main() {

	int graf[N][N] = {
		{0, 2, 4, 0, 0},
		{2, 0, 1, 7, 0},
		{4, 1, 0, 0, 3},
		{0, 7, 0, 0, 1},
		{0, 0, 3, 1, 0}
	};

	dijkstra(graf, 0);

	return 0;
}