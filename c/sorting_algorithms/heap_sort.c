#include<stdio.h>
#include<stdlib.h>

void displayArray(int* array,int n) {
  printf("displaying array\n");
  for(int i=0;i<n;i++) {
    printf("%d ",array[i]);
  }
  printf("\n");
}

void adjustRecursive(int* array,int rootIndex,int lastIndex) {
  //This algorithm takes a tree as input whose immediate right and left child
  //are max-heap;
  int leftChildIndex = 2*rootIndex+1;
  int rightChildIndex = 2*rootIndex+2;

  if(leftChildIndex > lastIndex) {// when there is no child node.
  //Note: here there is no need to check for existence of right child because
  //the adjust algorith requires that the immediate right and left child
  //of the root are max heap
    return;
  }
    
  int swapIndex; // to store the index with which the root element is to be swapped to create max-heap
  if(rightChildIndex > lastIndex) //when there is no right child swapindex = leftchildindex
    swapIndex = leftChildIndex;
  else // swapindex will be index(max(righchild,leftchild))
    swapIndex = array[leftChildIndex]>array[rightChildIndex]?leftChildIndex:rightChildIndex;

  //check if swap is required or not in case the root is already greate than the element at swap index then
  //no swap is required
  if(array[swapIndex] > array[rootIndex]) {
    //swap element at swapIndex and rootIndex;
    int temp = array[swapIndex];
    array[swapIndex] = array[rootIndex];
    array[rootIndex] = temp;
  }
  else
    return;

  //recursively adjust the elements at
  adjustRecursive(array,swapIndex,lastIndex);
}

void heapify(int* array,int lastIndex) {
  int rootIndex,temp = lastIndex;
  while(temp>0) {
    //printf("temp : %d ",temp);
    rootIndex = (temp%2==0)?(temp/2 - 1):(temp/2) ;
    //printf("rootIndex : %d",rootIndex);
    adjustRecursive(array,rootIndex,lastIndex);
    temp = temp -1;
  }
}

void heapsort(int* array,int n) {
  int end = n-1;
  heapify(array,end);
  while(end>=0) {
    //swap array[0] and array[end]
    int temp = array[0];
    array[0] = array[end];
    array[end] = temp;
    adjustRecursive(array,0,end-1);

    end = end-1;
  }
}

int main() {
  printf("Enter the number of elements\n");
  int n;
  scanf("%d",&n);
  int *array = (int*)malloc(sizeof(int)*n);

  printf("Enter elements\n");
  for(int i=0;i<n;i++) {
    scanf("%d",array+i);
  }
  heapsort(array,n);
  displayArray(array,n);
}
