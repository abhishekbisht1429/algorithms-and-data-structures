#include <stdio.h>
#include <stdlib.h>

int swap(int* arr,int pos1,int pos2) {
  int temp = arr[pos1];
  arr[pos1] = arr[pos2];
  arr[pos2] = temp;
}

void displayArray(int *array,int n) {
  for(int i=0;i<n;i++) {
    printf("%d ",array[i]);
  }
  printf("\n");
}

int partition(int* arr,int start,int end) {
  int pivot = arr[end];
  int x = start-1;
  for(int i=start;i<end;++i) {
    if(arr[i]<pivot) {
      ++x;
      swap(arr,i,x); //swap i and x
    }
  }
  swap(arr,end,x+1); //x+1 is the correct index of pivot
  return x+1;
}

void quick_sort(int* arr,int start,int end) {
  if(start>=end)
    return;
  //pivot index
  int pi = partition(arr,start,end);
  quick_sort(arr,start,pi-1);
  quick_sort(arr,pi+1,end);
}


int main() {
  printf("Enter no. of elements\n");
  int n;
  scanf("%d",&n);
  printf("Enter %d elements to be sorted\n",n);
  int *array = (int*)malloc(sizeof(int)*n);
  for(int i=0;i<n;i++) {
    scanf("%d",array+i);
  }
  swap(array,0,2);
  quick_sort(array,0,n-1);
  printf("Sorted elements\n");
  displayArray(array,n);
}
