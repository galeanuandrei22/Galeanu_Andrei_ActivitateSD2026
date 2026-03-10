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

void inserareInceput(struct Nod** head, int valoare) {
	struct Nod* nodNou = creareNod(valoare);

	nodNou->next = *head;
	*head = nodNou;
}

void afisareLista(struct Nod* head) {
	struct Nod* temp = head;

	while (temp != NULL) {
		printf("%d ", temp->data);
		temp = temp->next;
	}
}

int main() {
	struct Nod* head = NULL;

	inserareInceput(&head, 10);
	inserareInceput(&head, 20);
	inserareInceput(&head, 30);

	printf("Elementele listei: ");
	afisareLista(head);

	return 0;
}