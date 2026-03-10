#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main() {
	int v[100], n, i, suma = 0, max, x, gasit = 0;

	printf("n = ");
	scanf("%d", &n);

	for (i = 0; i < n; i++) {
		printf("v[%d] = ", i);
		scanf("%d", &v[i]);
	}

	printf("Vectorul este: \n");

	for (i = 0; i < n; i++) {
		printf("%d \n", v[i]);
	}

	for (i = 0; i < n; i++) {
		suma += v[i];
	}

	printf("Suma = %d\n", suma);

	max = v[0];
	for(i = 1; i < n; i++) {
		if (v[i] > max) {
			max = v[i];
		}
	}

	printf("Max = %d\n", max);

	printf("Element cautat: ");
	scanf("%d", &x);

	for(i = 0; i < n; i++) {
		if (v[i] == x) {
			gasit = 1;
			break;
		}
	}

	if(gasit) printf("Elementul %d a fost gasit in vector.\n", x);
	else printf("Elementul %d nu a fost gasit in vector.\n", x);

	return 0;
}