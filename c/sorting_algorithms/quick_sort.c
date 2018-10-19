#include<stdio.h>
#include<stdlib.h>

int partition(int*,int,int);
void quick_sort(int *array,int start,int end) {
  if(start>=end)
    return;
  //partiton will search the appropriate positon for the first
  //element and place it there and return the position where the
  //element was placed
  int j = partition(array,start,end);
  //after one element has been placed at appropriate postion
  //we have to sort the remaining subarrays
  //left sub array - start to j-1
  //righ sub array - j+1 to end
  printf("start %d\nend %d\n",start,end);
  printf("j %d\n",j);
  quick_sort(array,start,j-1);
  quick_sort(array,j+1,end);
}

int partition(int* array,const int start,const int end) {
  printf("start_p : %d\nend_p : %d\n\n",start,end);
  int key = array[start];
  int i=start;
  int j=end;
  while(i<j) {
    printf("i %d  j %d\n",i,j);
    while(array[i]<key)
      i++;//keep skipping until an element which is greater than or equal to key is found
    while(array[j]>key)
      j--;//keep skiping elements from revers until an element which is smaller than or equal to key is found;
    //swap elements at position i and j - i contains an element greater than key
    // and j contains an element smaller than key
    if(i<j) { // swap elements only if looping condidition is still true
      //swapping otherwise will not lead to the desired partitions of given array
      printf("swapping i and j\n");
      int temp = array[i];
      array[i] = array[j];
      array[j] = temp;
    }
  }
  //swap key with element at j
  array[start] = array[j];
  array[j] = key;
  //NOTE : -here element at j is swapped instead of element at i
  //because after the loop above terminates, value at position i will be greater than key
  //and value at postion j will be lesser than key
  //so swapping value at j and key will result in the desired condition, i.e all elements to there
  //left of key are smaller than the elements to the right of key

  //return the postion of the correctly placed element
  return j;
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

  quick_sort(array,0,n-1);
  printf("Sorted elements\n");
  displayArray(array,n);
}
