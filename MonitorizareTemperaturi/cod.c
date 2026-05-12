#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct Nod {
	int ts;
	float temp;
	struct Nod* st, * dr;
	int h;
} Nod;

int inaltime(Nod* n) {
	return n ? n->h : 0;
}

int maxx(int a, int b) {
	return (a > b) ? a : b;
}

Nod* creeazaNod(int ts, float temp) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->ts = ts;
	nou->temp = temp;
	nou->st = nou->dr = NULL;
	nou->h = 1;
	return nou;
}

Nod* rotatieDreapta(Nod* y) {
	Nod* x = y->st;
	Nod* T2 = x->dr;
	x->dr = y;
	y->st = T2;
	y->h = maxx(inaltime(y->st), inaltime(y->dr)) + 1;
	x->h = maxx(inaltime(x->st), inaltime(x->dr)) + 1;
	return x;
}

Nod* rotatieStanga(Nod* x) {
	Nod* y = x->dr;
	Nod* T2 = y->st;
	y->st = x;
	x->dr = T2;
	x->h = maxx(inaltime(x->st), inaltime(x->dr)) + 1;
	y->h = maxx(inaltime(y->st), inaltime(y->dr)) + 1;
	return y;
}

int getEchilibru(Nod* n) {
	return n ? inaltime(n->st) - inaltime(n->dr) : 0;
}

Nod* insereaza(Nod* nod, int ts, float temp) {
	if (nod == NULL) return creeazaNod(ts, temp);
	if (ts < nod->ts) nod->st = insereaza(nod->st, ts, temp);
	else if (ts > nod->ts) nod->dr = insereaza(nod->dr, ts, temp);
	else return nod;

	nod->h = 1 + maxx(inaltime(nod->st), inaltime(nod->dr));
	int echilibru = getEchilibru(nod);

	if (echilibru > 1 && ts < nod->st->ts) return rotatieDreapta(nod);
	if (echilibru < -1 && ts > nod->dr->ts) return rotatieStanga(nod);
	if (echilibru > 1 && ts > nod->st->ts) {
		nod->st = rotatieStanga(nod->st);
		return rotatieDreapta(nod);
	}
	if (echilibru < -1 && ts < nod->dr->ts) {
		nod->dr = rotatieDreapta(nod->dr);
		return rotatieStanga(nod);
	}
	return nod;
}

void salveazaDescrescator(Nod* r, FILE* f) {
	if (r) {
		salveazaDescrescator(r->dr, f);
		fprintf(f, "ts: %d | temperatura: %.1f\n", r->ts, r->temp);
		salveazaDescrescator(r->st, f);
	}
}

int main() {
	FILE* fin = fopen("meteo.txt", "r");
	FILE* fout = fopen("raport.txt", "w");
	if (!fin || !fout) return 1;

	Nod* radacina = NULL;
	int n, ts;
	float temp;

	fscanf(fin, "%d", &n);
	for (int i = 0; i < n; i++) {
		fscanf(fin, "%d %f", &ts, &temp);
		radacina = insereaza(radacina, ts, temp);
	}

	fprintf(fout, "inaltime arbore avl: %d\n", inaltime(radacina));
	fprintf(fout, "date descrescator\n");
	salveazaDescrescator(radacina, fout);

	fclose(fin);
	fclose(fout);
	printf("s-a generat raportul in raport.txt\n");
	return 0;
}