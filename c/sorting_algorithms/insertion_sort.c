#include<stdio.h>
#include<stdlib.h>

void insertion_sort(int *array,int n) {
  for(int i=0;i<n;i++) {
    int temp = array[i];// We have to put the element at position i
    // into right place in the sorted
    //sub-array which is to the left on this element
    int j = i-1;
    while(j>=0 && array[j]>temp) {
      //shift the elements to right till appropriate
      //position for temp is found
      array[j+1] = array[j];
      j--;
    }
    //place the value inside temp at appropirate position
    array[j+1] = temp;
  }
}

void displayArray(int *array,int n) {
  for(int i=0;i<n;i++) {
    printf("%d ",array[i]);
  }
  printf("\n");
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

  insertion_sort(array,n);
  printf("Sorted elements\n");
  displayArray(array,n);
}
