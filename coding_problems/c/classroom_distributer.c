#include<stdio.h>
#include<stdlib.h>

struct class {
  int classId;
  int start;
  int end;
};

typedef struct class CLASS;
int compare(const void*,const void*);

int find_resource_allocated(CLASS* array,int n) {
  //sort classes based on the start time
  qsort(array,n,sizeof(CLASS),compare);
  //resource_occupancy holds the end time of the class which is allocated to a particular resources
  //identified here by the index of the array
  //max no of possible resources are equal to the total number of classes therefore size of
  //resource resource_occupancy  is n
  int *resource_occupancy = (int*)calloc(n,sizeof(int));

  //count will hold the nubmer of resources allocated
  int count = 1;

  //allocate first resource(here classroom) to first class
  resource_occupancy[0] = array[0].end;

  //start allocation of other resources
  for(int i=1;i<n;i++) {
    int j;
    //this loop below will check for every resource whether it can be allocated to the
    //current class. It is done by checking whether the end time of the most recent class
    //to which this resource is allocated is greater than start time of the current class.
    //If yes then - overlapping occurs so the resource cannot be allocted otherwise it is allocated
    for(j=0;j<count;j++) {
      if(resource_occupancy[j] < array[i].start) {
        resource_occupancy[j] = array[i].end;
        break;
      }
    }

    //if j==count it means no classroom that has been allocated till now can be allocated to this
    //current class, so allocate new resource for it.
    if(j==count)
      count++;
  }

  return count;
}

int compare(const void* a,const void* b) {
  return ((CLASS*)a)->start - ((CLASS*)b)->start;
}

int main() {
  printf("Enter the number of classes : ");
  int n;
  scanf("%d",&n);
  CLASS* array = (CLASS*)malloc(sizeof(CLASS)*n);
  for(int i=0;i<n;i++) {
    printf("class id : ");
    scanf("%d",&(array[i].classId));
    printf("start time : ");
    scanf("%d",&(array[i].start));
    printf("end time : ");
    scanf("%d",&(array[i].end));
    printf("\n");
  }

  int resources_allocated = find_resource_allocated(array,n);
  printf("number of resources allocated : %d\n",resources_allocated);
}
