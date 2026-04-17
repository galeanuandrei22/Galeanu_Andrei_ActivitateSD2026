#define _CRT_SECURE_NO_WARNINGS
#define SIZE 5
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
	int x, y;
} Point;

typedef struct {
	Point data[SIZE * SIZE];
	int top;
} Stack;

void push(Stack* s, Point p) {
	s->data[++(s->top)] = p;
}

Point pop(Stack* s) {
	return s->data[(s->top)--];
}

bool isEmpty(Stack* s) {
	return s->top == -1;
}

void printLabirint(int lab[SIZE][SIZE]) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (lab[i][j] == 2) printf(". ");
			else if (lab[i][j] == 1) printf("# ");
			else printf(" ");
		}
		printf("\n");
	}
}

bool isSafe(int lab[SIZE][SIZE], int x, int y) {
	return (x >= 0 && x < SIZE && y >= 0 && y < SIZE && lab[x][y] == 0);
}

bool solveMaze(int lab[SIZE][SIZE], Point start, Point end) {
	Stack s;
	s.top = -1;
	push(&s, start);
	int dx[] = { -1, 1, 0, 0 };
	int dy[] = {0, 0, -1 ,1};

	while (!isEmpty(&s)) {
		Point curr = pop(&s);

		if (curr.x == end.x && curr.y == end.y) {
			lab[curr.x][curr.y] = 2;
			return true;
		}

		if (isSafe(lab, curr.x, curr.y)) {
			lab[curr.x][curr.y] = 2;
			for (int i = 0; i < 4; i++) {
				Point next = { curr.x + dx[i], curr.y + dy[i] };
				if (isSafe(lab, next.x, next.y)) {
					push(&s, next);
				}
			}
		}
	}
	return false;
}

int main() {
	int labirint[SIZE][SIZE] = {
		{0, 1, 0, 0, 0},
		{0, 1, 0, 1, 0},
		{0, 0, 0, 1, 0},
		{1, 1, 0, 0, 0},
		{0, 0, 0, 1, 0}
	};

	Point start = { 0,0 };
	Point end = { 4,4 };
	printf("Labirintul initial:\n");
	printLabirint(labirint);

	if (solveMaze(labirint, start, end)) {
		printf("\nIesire gasita!\n");
		printLabirint(labirint);
	}
	else {
		printf("\nNu exista cale de iesire.\n");
	}

	return 0;
}
