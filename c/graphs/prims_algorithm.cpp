#include<iostream>
#include<list>
#include<limits>

using namespace std;

typedef struct Graph {
  int v;
  int** graph;
  bool directed;
  Graph(int v,bool directed) {
    this->v = v;
    graph = new int*[v];
    for(int i=0;i<v;++i) {
      graph[i] = new int[v];
    }
    this->directed = directed;
  }

  void addEdge(int u,int v,int w) {
    if(u>=this->v || v>=this->v) {
      cout<<"Invalid Edge";
      return;
    }
    graph[u][v] = w;
    if(!directed)
      graph[v][u] = w;
  }

  void displayGraph();
  int primsMst(int* parentSet);

} Graph;

void Graph::displayGraph() {
  for(int i=0;i<v;++i) {
    for(int j=0;j<v;++j) {
      printf("%-3d ",graph[i][j]);
    }
    cout<<endl;
  }
}
int Graph::primsMst(int* parentSet) {
  int minKeyVertex(int*,bool*,int);
  bool* mstSet = new bool[v];
  int* keySet = new int[v];

  //intialization
  for(int i=0;i<v;++i) {
    parentSet[i] = -1; //set the parent as -1 (not defined) for all vertices
    keySet[i] = numeric_limits<int>::max(); //set the key as infinity for all
    mstSet[i] = false;
  }
  keySet[0] = 0;

  //run the main algorithm
  for(int i=1;i<v;++i) {
    //u holds the vertex with minimum key
    int u = minKeyVertex(keySet,mstSet,v);
    //mark node u as included
    mstSet[u] = true;

    //update the key values of nodes adjacent to u
    for(int j=0;j<v;++j) {
      if(mstSet[j] == false && graph[u][j]!=0 && graph[u][j]<keySet[j]) {
        parentSet[j] = u;
        keySet[j] = graph[u][j];
      }
    }
  } //end of main algorithm
}

int minKeyVertex(int* keySet,bool* mstSet,int n) {
  int min = numeric_limits<int>::max();
  int minKeyVertex = -1;
  for(int i=0;i<n;++i) {
    if(!mstSet[i] && (keySet[i] < min) ) {
      minKeyVertex = i;
    }
  }
  return minKeyVertex;
}

void printMST(int* parentSet,int n) {
  cout<<"Edges of MST\n";
  for(int i=1;i<n;++i) {
    cout<<parentSet[i]<<" -> "<<i<<endl;
  }
}

int main() {
  int v = 8;
  Graph graph(v,false);
  graph.addEdge(0,1,14);
  graph.addEdge(0,2,6);
  graph.addEdge(0,4,5);
  graph.addEdge(0,3,10);
  graph.addEdge(1,3,3);
  graph.addEdge(2,4,4);
  graph.addEdge(3,5,8);
  graph.addEdge(4,6,9);
  graph.addEdge(4,5,2);
  graph.addEdge(5,7,15);


  graph.displayGraph();

  int* parentSet = new int[v];
  graph.primsMst(parentSet);

  printMST(parentSet,v);
}
