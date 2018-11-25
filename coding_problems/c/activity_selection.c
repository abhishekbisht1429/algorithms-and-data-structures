#include<stdlib.h>
#include<stdio.h>

typedef struct Activity {
  int id;
  int start;
  int finish;
} ACTIVITY;
int compare(const void*,const void*);

ACTIVITY *select_activities(ACTIVITY* array,int n) {
  ACTIVITY *solution_vector = (ACTIVITY*)calloc(n,sizeof(ACTIVITY));
  qsort(array,n,sizeof(ACTIVITY),compare);

  solution_vector[0] = array[0];
  int count = 1;
  for(int i=1;i<n;i++) {
    if(array[i].start > solution_vector[count-1].finish) {
      solution_vector[count] = array[i];
      count++;
    }
  }

  return solution_vector;
}

int compare(const void* a,const void* b) {
  return ((ACTIVITY*)a)->finish - ((ACTIVITY*)b)->finish;
}

int main() {
  printf("Enter the number of activities : ");
  int n;
  scanf("%d",&n);
  ACTIVITY* array = (ACTIVITY*)malloc(sizeof(ACTIVITY)*n);
  for(int i=0;i<n;i++) {
    printf("Activity id : ");
    scanf("%d",&(array[i].id));
    printf("start time : ");
    scanf("%d",&(array[i].start));
    printf("end time : ");
    scanf("%d",&(array[i].finish));
    printf("\n");
  }
  ACTIVITY *solution = select_activities(array,n);
  printf("Solution vector\n");
  for(int i=0;i<n && solution[i].id!=0;i++) {
    printf("%d ",solution[i].id);
  }
}
