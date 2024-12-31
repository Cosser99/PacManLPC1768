#ifndef PQueue
#define PQueue
#include "LPC17xx.h"
#define MAX_SIZE 100  // Dimensione massima della priority queue

typedef struct {
    uint8_t data[MAX_SIZE];	//distanza
	
    uint16_t size;  // Numero di elementi nella queue
} PriorityQueue;



void initQueue(PriorityQueue *pq);
int parent(int i);
int leftChild(int i) ;
int rightChild(int i);
void swap(int *a, int *b);
int insert(PriorityQueue *pq, uint8_t *value);
void heapifyDown(PriorityQueue *pq, int i);
int extractMax(PriorityQueue *pq);
int isEmpty(PriorityQueue *pq) ;
#endif