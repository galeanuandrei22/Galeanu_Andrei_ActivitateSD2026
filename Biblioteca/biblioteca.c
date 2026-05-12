#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Nod {
	int isbn;
	char titlu[50];
	struct Nod* st, * dr;
} Nod;

Nod* creeazaNod(int isbn, char* titlu) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->isbn = isbn;
	strcpy(nou->titlu, titlu);
	nou->st = nou->dr = NULL;
	return nou;
}

Nod* minim(Nod* nod) {
	Nod* curent = nod;
	while (curent && curent->st != NULL) {
		curent = curent->st;
	}
	return curent;
}

Nod* insereaza(Nod* radacina, int isbn, char* titlu) {
	if (radacina == NULL) return creeazaNod(isbn, titlu);
	if (isbn < radacina->isbn) radacina->st = insereaza(radacina->st, isbn, titlu);
	else if (isbn > radacina->isbn) radacina->dr = insereaza(radacina->dr, isbn, titlu);
	return radacina;
}

Nod* sterge(Nod* radacina, int isbn) {
	if (radacina == NULL) return radacina;
	if (isbn < radacina->isbn) radacina->st = sterge(radacina->st, isbn);
	else if (isbn > radacina->isbn) radacina->dr = sterge(radacina->dr, isbn);
	else {
		if (radacina->st == NULL) {
			Nod* temp = radacina->dr;
			free(radacina);
			return temp;
		}
		else if (radacina->dr == NULL) {
			Nod* temp = radacina->st;
			free(radacina);
			return temp;
		}
		Nod* temp = minim(radacina->dr);
		radacina->isbn = temp->isbn;
		strcpy(radacina->titlu, temp->titlu);
		radacina->dr = sterge(radacina->dr, temp->isbn);
	}
	return radacina;
}

void salveazaPreordine(Nod* r, FILE* f) {
	if (r) {
		fprintf(f, "%d %s\n", r->isbn, r->titlu);
		salveazaPreordine(r->st, f);
		salveazaPreordine(r->dr, f);
	}
}

int main() {
	FILE* fin = fopen("carti.txt", "r");
	if (!fin) {
		printf("eroare la deschiderea fisierului carti.txt\n");
		return 1;
	}

	Nod* radacina = NULL;
	int n, isbn;
	char titlu[50];

	fscanf(fin, "%d", &n);
	for (int i = 0; i < n; i++) {
		fscanf(fin, "%d %s", &isbn, titlu);
		radacina = insereaza(radacina, isbn, titlu);
	}
	fclose(fin);

	radacina = sterge(radacina, 500);

	FILE* fout = fopen("carti2.txt", "w");
	if (fout) {
		salveazaPreordine(radacina, fout);
		fclose(fout);
		printf("fisier carti2 generat.\n");
	}

	return 0;
}