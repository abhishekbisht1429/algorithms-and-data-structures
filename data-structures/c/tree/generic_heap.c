#include<stdio.h>
#include<stdlib.h>
#include<math.h>

struct Heap {
    void **arr; //pointer to an array of void type pointers
    int capacity;
    int size;
    int (*compare)(void*, void*);
};
typedef struct Heap HEAP;

HEAP *newHeap(int capacity, int (*compare)(void*, void*)) {
    HEAP *heap = (HEAP*)malloc(sizeof(HEAP));
    heap->arr = malloc(sizeof(void*) * capacity);
    heap->capacity = capacity;
    heap->size = 0;
    heap->compare = compare;
    return heap;
}

void swap(void **x, void **y) {
    void* temp = *x;
    *x = *y;
    *y = temp;
}

void heapifyUp(HEAP *heap, int pos) {
    int parent = (pos - 1)/2;
    
    //while(heap->arr[parent] > heap->arr[pos]) { //comparison
    while(heap->compare(heap->arr[parent], heap->arr[pos])) {
        swap(&(heap->arr[parent]), &(heap->arr[pos]));
        pos = parent;
        parent = (parent - 1)/2;
    }
}


void heapifyDown(HEAP *heap, int pos) {
    int lcPos = 2*pos+1;
    int rcPos = 2*pos+2;
    while(lcPos < heap->size || rcPos < heap->size) {
        int ltPos;
        if(lcPos >= heap->size) {
            ltPos = rcPos;
        } else if(rcPos >= heap->size) {
            ltPos = lcPos;
        } else {
            //ltPos = heap->arr[lcPos]<heap->arr[rcPos]?lcPos:rcPos;
            ltPos = heap->compare(heap->arr[lcPos], heap->arr[rcPos])?lcPos:rcPos;
        }
        // if(heap->arr[ltPos] < heap->arr[pos]) {
        if(heap->compare(heap->arr[ltPos], heap->arr[pos])) {
            swap(&(heap->arr[ltPos]), &(heap->arr[pos]));
        }
        pos = ltPos;
        lcPos = 2*pos+1;
        rcPos = 2*pos+2;
    }
}

void delete_top(HEAP *heap) {
    swap(&(heap->arr[0]), &(heap->arr[heap->size-1]));
    heap->size = heap->size - 1;
    heapifyDown(heap, 0);
}

void insert(HEAP *heap, void *key) {
    if(heap->size == heap->capacity) {
        printf("Heap full\n");
        return;
    } else {
        heap->size = heap->size + 1;
        int i = heap->size - 1;
        heap->arr[i] = key;
        heapifyUp(heap, i);
    }
}

void display(HEAP *heap) {
    for(int i=0;i<heap->capacity; ++i) {
        printf("%p ", heap->arr[i]);
    }
    printf("\n");
}

void displayTree(HEAP *heap) {
    int h = (int)(log2(heap->size)) + 1;
    for(int l=0; l<h; ++l) {
        for(int i=0;i<(int)pow(2, h-l-1); ++i) {
            printf(" ");
        }
        for(int i=(int)pow(2,l)-1; i<heap->size && i<(int)pow(2, l+1)-1; ++i) {
            printf("%p",heap->arr[i]);
            for(int j=1;j<=(int)pow(2, h-l)-1; ++j) {
                printf(" ");
            }
        }
        printf("\n");
    }
}

int compareInt(void *p1, void *p2) { //function to compare integers
    int *x = (int*)p1;
    int *y = (int*)p2;

    return *x - *y;
}

int main() {
    HEAP *heap = newHeap(255, compareInt);

    int values[9];
    for(int i=1;i<=9;++i) {
        values[i] = i;
        insert(heap, &(values[i]));
    }

    displayTree(heap);

    delete_top(heap);

    displayTree(heap);
    return 0;
}
