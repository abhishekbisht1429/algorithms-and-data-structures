#include<iostream>

using namespace std;

int max(int a, int b) {
  return a>b?a:b;
}
void findSolVec(int* solVec,int** profitMatrix,int* weight,int n,int maxW) {
  int i=n;
  int j = maxW;
  while(i>0 && j>0) {
    if(profitMatrix[i][j]!=profitMatrix[i-1][j]) {
      solVec[i-1] = 1;
      j = j-weight[i-1];
    } else {
      --i;
    }
  }
}
int solveKnapsack(int* solVec,int* weight,int* profit,int n,int maxW) {
  int** profitMatrix = new int*[n+1];
  for(int i=0;i<=n;++i)
    profitMatrix[i] = new int[maxW+1];
  for(int i=0;i<=n;++i) {
    for(int j=0;j<=maxW;++j) {
      if(i==0 || j==0) {
        profitMatrix[i][j] = 0;
      } else if(weight[i-1]>j) {
        profitMatrix[i][j] = profitMatrix[i-1][j];
      } else {
        profitMatrix[i][j] = max(profitMatrix[i-1][j],
                                profit[i-1]+profitMatrix[i-1][j-weight[i-1]]);
      }
      printf("%d ",profitMatrix[i][j]);
    }
    cout<<endl;
  }
  findSolVec(solVec,profitMatrix,weight,n,maxW);
  return profitMatrix[n][maxW];
}

int main() {
  int weight[] = {2,3,4,5};
  int profit[] = {3,4,5,6};
  int n = sizeof(weight)/sizeof(weight[0]);
  int maxW = 5;
  int* solVec = new int[n];
  printf("total profit : %d\n",solveKnapsack(solVec,weight,profit,n,maxW));
  printf("Solution Vector\n");
  for(int i=0;i<n;++i) {
    printf("%d ",solVec[i]);
  }
  cout<<endl;
}
