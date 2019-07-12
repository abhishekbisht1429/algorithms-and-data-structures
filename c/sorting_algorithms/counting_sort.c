#include<stdio.h>
#include<stdlib.h>

int findMax(int*,int);
void counting_sort(int* array,int n) {
  int max = findMax(array,n);
  int *freq_array = (int*)calloc(max+1,sizeof(int));
  for(int i=0;i<n;i++)
    freq_array[array[i]]++;

  int pos = 0;//pos represents the index of the array in which sorted elements
  //are to be placed(here we are reusing the original array for storing sorted elements)
  for(int i=0;i<max+1;i++) {
    //put the 'i'th element 'freq_array[i]'' number of time in 'array'
    for(int j=0;j<freq_array[i];j++) {
      array[pos] = i;
      pos++;
    }
  }
}
int findMax(int* array,int n) {
  int max=array[0];
  for(int i=1;i<n;i++)
    if(array[i]>max)
      max = array[i];

  return max;
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

  counting_sort(array,n);
  printf("Sorted elements\n");
  displayArray(array,n);
}
