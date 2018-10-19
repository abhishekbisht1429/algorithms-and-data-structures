#include<stdio.h>

int linear_search(int* array,int n,int x) {
  //this function will return the postion of the element if it is found
  // otherwise it will return -1;
  for(int i=0;i<n;i++)
    if(array[i]==x)
      return i;

  return -1;
}

int main() {
  int array[] = {1,2,4,3,7,5,6,8,10,9};
  int size = 10;
  printf("Enter the element to be searched\n");
  int x;
  scanf("%d",&x);

  printf("index of the element : %d\n",linear_search(array,size,x));
}
