#include<stdio.h>
#include<stdlib.h>

void merge(int*,int,int,int); //declaration of merge function
// This merge sort sorts the elements in place
void merge_sort(int *array,int start,int end) {
  if(start>=end) {
    return;
  }
  int mid = (start+end)/2;
  merge_sort(array,start,mid);
  merge_sort(array,mid+1,end);

  merge(array,start,mid,end);
}

void merge(int *array,int start,int mid,int end) {
  int i=start;
  int j=mid+1;
  int *temp_array = (int*)malloc(sizeof(int)*(end-start+1));
  int k=0;

  //merging the two sorted sub-arrays
  //1st subarray - start to mid
  //2nd subarray - mid+1 to end
  while(i<=mid && j<=end) {
    if(array[i]<=array[j]) {
      temp_array[k]=array[i];
      i++;
    }
    else {
      temp_array[k]=array[j];
      j++;
    }
    k++;
  }

  //put the remainng elements into the array
  if(i>mid)
    while(j<=end) {
      temp_array[k] = array[j];
      j++;
      k++;
    }
  else if(j>end)
    while(i<=mid) {
      temp_array[k] = array[i];
      i++;
      k++;
    }

  //copy the elements into the original array from temp_array
  for(int x=start;x<=end;x++)
    array[x] = temp_array[x-start];

  //delete the temp_array
  free(temp_array);
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

  merge_sort(array,0,n-1);
  printf("Sorted elements\n");
  displayArray(array,n);
}
