#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Statie {
	int idStatie;
	int consumCurent;
	float tensiuneOperare;
	char* localitate;
	char* responsabil;
	unsigned char codZona;
};
typedef struct Statie Statie;

struct Nod {
	Statie info;
	struct Nod* st;
	struct Nod* dr;
};
typedef struct Nod Nod;

void afisareStatie(Statie s) {
	printf("ID: %d | Loc: %s | Responsabil: %s | Consum: %d MW | Tensiune: %.1f kV | Zona: %c\n",
		s.idStatie, s.localitate, s.responsabil, s.consumCurent, s.tensiuneOperare, s.codZona);
}

Statie initStatie(int id, int consum, float tensiune, const char* loc, const char* resp, unsigned char zona) {
	Statie s;
	s.idStatie = id;
	s.consumCurent = consum;
	s.tensiuneOperare = tensiune;
	s.codZona = zona;

	s.localitate = (char*)malloc(strlen(loc) + 1);
	strcpy(s.localitate, loc);

	s.responsabil = (char*)malloc(strlen(resp) + 1);
	strcpy(s.responsabil, resp);

	return s;
}

void adaugaStatieInArbore(Nod** rad, Statie statieNoua) {
	if (*rad == NULL) {
		Nod* nod = malloc(sizeof(Nod));
		nod->info = statieNoua;
		nod->dr = NULL;
		nod->st = NULL;
		*rad = nod;
	}
	else {
		if ((*rad)->info.idStatie > statieNoua.idStatie) {
			adaugaStatieInArbore(&((*rad)->st), statieNoua);
		}
		if ((*rad)->info.idStatie < statieNoua.idStatie) {
			adaugaStatieInArbore(&((*rad)->dr), statieNoua);
		}
	}
}
Nod* citireArboreDinFisier(const char* numeFisier) {
	Nod* rad = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		char buffer[150];
		char sep[3] = ",\n";
		while (fgets(buffer, 150, f)) {
			char* aux = strtok(buffer, sep);
			int id = atoi(aux);
			int consum = atoi(strtok(NULL, sep));
			float tensiune = (float)atof(strtok(NULL, sep));

			aux = strtok(NULL, sep);
			char* loc = malloc(strlen(aux) + 1);
			strcpy(loc, aux);

			aux = strtok(NULL, sep);
			char* resp = malloc(strlen(aux) + 1);
			strcpy(resp, aux);

			unsigned char zona = *strtok(NULL, sep);

			Statie s = initStatie(id, consum, tensiune, loc, resp, zona);
			adaugaStatieInArbore(&rad, s);

			free(loc);
			free(resp);
		}
		fclose(f);
	}
	return rad;
}

void afisareMasiniDinArbore(Nod* rad) { 
	if (rad) {
		afisareMasiniDinArbore(rad->st);
		afisareStatie(rad->info);
		afisareMasiniDinArbore(rad->dr);
	}
}

int calculeazaConsumLocalitate(Nod* rad, const char* localitate) {
	if (rad) {
		int consumCurentNod = 0;
		if (strcmp(rad->info.localitate, localitate) == 0) {
			consumCurentNod = rad->info.consumCurent;
		}
		return consumCurentNod +
			calculeazaConsumLocalitate(rad->st, localitate) +
			calculeazaConsumLocalitate(rad->dr, localitate);
	}
	return 0;
}

int numaraStatiiPestePragTensiune(Nod* rad, float prag) {
	if (rad) {
		int contor = 0;
		if (rad->info.tensiuneOperare > prag) {
			contor = 1;
		}
		return contor +
			numaraStatiiPestePragTensiune(rad->st, prag) +
			numaraStatiiPestePragTensiune(rad->dr, prag);
	}
	return 0;
}

void dezalocareArbore(Nod** rad) {
	if (*rad) {
		dezalocareArbore(&(*rad)->st);
		dezalocareArbore(&(*rad)->dr);
		free((*rad)->info.localitate);
		free((*rad)->info.responsabil);
		free(*rad);
		*rad = NULL;
	}
}

int main() {
	Nod* rad = citireArboreDinFisier("statii.txt");

	printf("--- Parcurgere Inordine (Sortat dupa ID-ul Statiei) ---\n");
	afisareMasiniDinArbore(rad);
	printf("\n");

	const char* orasCautat = "Brasov";
	printf("Consumul total in localitatea %s: %d MW\n",
		orasCautat, calculeazaConsumLocalitate(rad, orasCautat));

	float limitaTensiune = 200.0f;
	printf("Numarul de statii care depasesc tensiunea de %.1f kV: %d\n",
		limitaTensiune, numaraStatiiPestePragTensiune(rad, limitaTensiune));

	dezalocareArbore(&rad);
	return 0;
}