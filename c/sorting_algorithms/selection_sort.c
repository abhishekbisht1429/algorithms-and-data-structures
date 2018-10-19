#include<stdio.h>
#include<stdlib.h>

void selection_sort(int* array,int n) {
  for(int i=0;i<n;i++) {
    int pos = i; // pos holds the position of minimum element in the array
    //this loop finds the minimum element in the array(index ranging from i+1 to n-1)
    //and stores its position in pos
    for(int j=i+1;j<n;j++) {
      if(array[pos]>array[j])
        pos = j;
    }
    //swap the ith minimum element(which is at postion pos)
    //with the element at the position i
    int temp = array[i];
    array[i] = array[pos];
    array[pos] = temp;
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

  selection_sort(array,n);
  printf("Sorted elements\n");
  displayArray(array,n);
}
