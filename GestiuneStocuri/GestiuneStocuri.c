#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct Product {
	int id;
	struct Product* left, * right;
} Product;

Product* createProduct(int id) {
	Product* newNode = (Product*)malloc(sizeof(Product));
	newNode->id = id;
	newNode->left = newNode->right = NULL;
	return newNode;
}

Product* insert(Product* root, int id) {
	if (root == NULL) return createProduct(id);
	if (id < root->id) root->left = insert(root->left, id);
	else if (id > root->id) root->right = insert(root->right, id);
	return root;
}

Product* search(Product* root, int id) {
	if (root == NULL || root->id == id) return root;
	if (id < root->id) return search(root->left, id);
	return search(root->right, id);
}

void displayInOrder(Product* root) {
	if (root != NULL) {
		displayInOrder(root->left);
		printf("ID produs: %d\n", root->id);
		displayInOrder(root->right);
	}
}

int main() {
	Product* inventory = NULL;
	inventory = insert(inventory, 50);
	insert(inventory, 32);
	insert(inventory, 58);
	insert(inventory, 21);
	insert(inventory, 87);
	insert(inventory, 25);
	insert(inventory, 73);

	printf("Inventarul complet (ordonat):\n");
	displayInOrder(inventory);
	int searchId = 73;
	if (search(inventory, searchId)) printf("\nProdusul %d a fost gasit in depozit.\n", searchId);
	else printf("\nProdusul %d nu exista.\n", searchId);
	return 0;
}