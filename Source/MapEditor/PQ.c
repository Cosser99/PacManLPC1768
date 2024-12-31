#include "PQ.h"
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>




// Funzione per inizializzare la priority queue
void initQueue(PriorityQueue *pq) {
    pq->size = 0;
}

// Funzione per restituire l'indice del genitore
int parent(int i) {
    return (i - 1) / 2;
}

// Funzione per restituire l'indice del figlio sinistro
int leftChild(int i) {
    return 2 * i + 1;
}

// Funzione per restituire l'indice del figlio destro
int rightChild(int i) {
    return 2 * i + 2;
}

// Funzione per scambiare due elementi
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Funzione per inserire un elemento nella priority queue
int insert(PriorityQueue *pq, uint8_t *value) {
    if (pq->size == MAX_SIZE) {
        return 0;
    }

    // Aggiungi l'elemento alla fine
    pq->data[pq->size] = *value;
    int i = pq->size;
    pq->size++;

    // Effettua il "bubble up" per mantenere la proprietà del heap
    while (i > 0 && pq->data[parent(i)] < pq->data[i]) {
        swap(&pq->data[parent(i)], &pq->data[i]);
        i = parent(i);
    }

    return 1;
}

// Funzione per effettuare il "heapify down"
void heapifyDown(PriorityQueue *pq, int i) {
    int largest = i;
    int left = leftChild(i);
    int right = rightChild(i);

    // Trova il più grande tra genitore, figlio sinistro e figlio destro
    if (left < pq->size && pq->data[left] > pq->data[largest]) {
        largest = left;
    }
    if (right < pq->size && pq->data[right] > pq->data[largest]) {
        largest = right;
    }

    // Se il più grande non è il genitore, scambia e continua
    if (largest != i) {
        swap(&pq->data[i], &pq->data[largest]);
        heapifyDown(pq, largest);
    }
}

// Funzione per rimuovere l'elemento con priorità massima
int extractMax(PriorityQueue *pq) {
    if (pq->size == 0) {
        return INT_MIN;
    }

    int maxValue = pq->data[0];  // La radice contiene il massimo
    pq->data[0] = pq->data[pq->size - 1];
    pq->size--;

    // Ripristina la proprietà del heap
    heapifyDown(pq, 
		0);

    return maxValue;
}

// Funzione per verificare se la priority queue è vuota
int isEmpty(PriorityQueue *pq) {
    return pq->size == 0;
}

