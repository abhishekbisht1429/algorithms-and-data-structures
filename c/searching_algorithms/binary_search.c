#include<stdio.h>

//Binary search algorithm requires the array(in which the element is to be searched)
// to be sorted
int binary_search_recursive(int *array,int start,int end,int x) {
  if(start>end)
    return -1;
  int mid = (start+end)/2;
  if(x > array[mid])
    return binary_search_recursive(array,mid+1,end,x);
  else if(x < array[mid])
    return binary_search_recursive(array,start,mid-1,x);
  else
    return mid;
}

int binary_search_iterative(int *array,int n,int x) {
  int start=0,end = n-1,mid;
  while(start<=end) {
    mid = (start+end)/2;
    if(x > array[mid])
      start = mid+1;
    else if(x < array[mid])
      end = mid-1;
    else
      return mid;
  }
  return -1;
}

int main() {
  // NOTE : - The array must be sorted in order to perform binary search on it
  int array[] = {1,2,3,4,5,6,7,8,9,10};
  int size = 10;
  printf("Enter the element to be searched\n");
  int x;
  scanf("%d",&x);

  printf("index of the element(using iterative binary search) : %d\n",binary_search_iterative(array,size,x));
  printf("index of the element(using recursive binary search) : %d\n",binary_search_recursive(array,0,size-1,x));
}
