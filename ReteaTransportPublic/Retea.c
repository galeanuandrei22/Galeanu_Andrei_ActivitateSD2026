#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
	int stationID;
	struct Node* next;
} Node;

typedef struct Graph {
	int numStations;
	Node** adjLists;
	bool* visited;
} Graph;

typedef struct Queue {
	int items[100];
	int front;
	int rear;
} Queue;

Queue* createQueue() {
	Queue* q = malloc(sizeof(Queue));
	q->front = -1;
	q->rear = -1;
	return q;
}

void enqueue(Queue* q, int value) {
	if (q->rear == 99) return;
	if (q->front == -1) q->front = 0;
	q->rear++;
	q->items[q->rear] = value;
}

int dequeue(Queue* q) {
	int item;
	if (q->front == -1) return -1;
	item = q->items[q->front];
	q->front++;
	if (q->front > q->rear) q->front = q->rear = -1;
	return item;
}

bool isEmpty(Queue* q) {
	return q->front == -1;
}

Node* createNode(int v) {
	Node* newNode = malloc(sizeof(Node));
	newNode->stationID = v;
	newNode->next = NULL;
	return newNode;
}

Graph* createGraph(int vertices) {
	Graph* graph = malloc(sizeof(Graph));
	graph->numStations = vertices;
	graph->adjLists = malloc(vertices * sizeof(Node*));
	graph->visited = malloc(vertices * sizeof(bool));

	for (int i = 0; i < vertices; i++) {
		graph->adjLists[i] = NULL;
		graph->visited[i] = false;
	}
	return graph;
}

void addEdge(Graph* graph, int src, int dest) {
	Node* newNode = createNode(dest);
	newNode->next = graph->adjLists[src];
	graph->adjLists[src] = newNode;
	newNode = createNode(src);
	newNode->next = graph->adjLists[dest];
	graph->adjLists[dest] = newNode;
}

void findShortestPath(Graph* graph, int startNode, int endNode) {
	Queue* q = createQueue();
	int* parent = malloc(graph->numStations * sizeof(int));
	for (int i = 0; i < graph->numStations; i++) {
		parent[i] = -1;
		graph->visited[i] = false;
	}
	graph->visited[startNode] = true;
	enqueue(q, startNode);

	while (!isEmpty(q)) {
		int currentStation = dequeue(q);
		if (currentStation == endNode) break;
		Node* temp = graph->adjLists[currentStation];
		while (temp) {
			int adjStation = temp->stationID;
			if (!graph->visited[adjStation]) {
				graph->visited[adjStation] = true;
				parent[adjStation] = currentStation;
				enqueue(q, adjStation);
			}
			temp = temp->next;
		}
	}
	if (parent[endNode] == -1 && startNode != endNode) {
		printf("Nu exista drum intre statia %d si statia %d\n", startNode, endNode);
	}
	else {
		printf("Cel mai scurt drum: ");
		int curr = endNode;
		int path[100], count = 0;
		while (curr != -1) {
			path[count++] = curr;
			curr = parent[curr];
		}
		for (int i = count - 1; i >= 0; i--) {
			printf("%d%s", path[i], (i == 0 ? "" : " -> "));
		}
		printf("\n");
	}
	free(parent);
	free(q);
}

int main() {
	int n = 6;
	Graph* reteaMetrou = createGraph(n);

	addEdge(reteaMetrou, 0, 1);
	addEdge(reteaMetrou, 0, 2);
	addEdge(reteaMetrou, 1, 3);
	addEdge(reteaMetrou, 2, 4);
	addEdge(reteaMetrou, 3, 4);
	addEdge(reteaMetrou, 3, 5);
	addEdge(reteaMetrou, 4, 5);

	printf("Se cauta ruta optima de la Statia 2 la Statia 5\n");
	findShortestPath(reteaMetrou, 2, 5);

	return 0;
}