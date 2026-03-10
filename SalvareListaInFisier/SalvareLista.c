#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

struct Nod {
	int data;
	struct Nod* next;
};

struct Nod* creareNod(int valoare) {
	struct Nod* nodNou = (struct Nod*)malloc(sizeof(struct Nod));

	nodNou->data = valoare;
	nodNou->next = NULL;

	return nodNou;
}

void inserareFinal(struct Nod** head, int valoare) {
	struct Nod* nodNou = creareNod(valoare);

	if (*head == NULL) {
		*head = nodNou;
		return;
	}

	struct Nod* temp = *head;

	while (temp->next != NULL) {
		temp = temp->next;
	}

	temp->next = nodNou;
}

void afisareLista(struct Nod* head) {
	while (head != NULL) {
		printf("%d ", head->data);
		head = head->next;
	}

	printf("\n");
}

void salvareListaFisier(struct Nod* head) {
	FILE* f;

	f = fopen("lista.txt", "w");

	if (f == NULL) {
		printf("eroare la deschiderea fisierului!");
		return;
	}

	while (head != NULL) {
		fprintf(f, "%d ", head->data);
		head = head->next;
	}

	fclose(f);
}

int main() {
	struct Nod* head = NULL;

	inserareFinal(&head, 5);
	inserareFinal(&head, 10);
	inserareFinal(&head, 15);
	inserareFinal(&head, 20);

	printf("Lista:\n");
	afisareLista(head);

	salvareListaFisier(head);
	printf("Lista a fost salvata in fisier!");
	return 0;
}