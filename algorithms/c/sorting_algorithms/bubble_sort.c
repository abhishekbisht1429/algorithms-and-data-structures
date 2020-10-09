#include<stdio.h>
#include<stdlib.h>

void bubble_sort(int* array,int n) {
  for(int i=0;i<n-1;i++)
    // the inner loop below after completion puts the maximum element
    // at n-i-1 th position
    // here the direction of sort is from end to start
    for(int j=0;j<n-i-1;j++) {
      //checks the adjacent elements and swaps them
      //if they are not in ascending order
      if(array[j]>array[j+1]){
        int temp = array[j];
        array[j] = array[j+1];
        array[j+1] = temp;
      }
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

  bubble_sort(array,n);
  printf("Sorted elements\n");
  displayArray(array,n);
}
