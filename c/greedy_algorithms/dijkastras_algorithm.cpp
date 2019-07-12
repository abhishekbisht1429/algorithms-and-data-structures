#include<iostream>
#include<limits>
#include<vector>
#define INF numeric_limits<int>::max()
using namespace std;

struct Graph {
  int** graph;
  int v;
  bool directed;
  Graph(int v,bool directed) {
    this->v = v;
    this->directed = directed;
    graph = new int*[v];
    for(int i=0;i<v;++i)
      graph[i] = new int[v];
  }

  void addEdge(int u,int v,int w) {
    if(u>=this->v || v>=this->v) {
      cout<<"Invalid Edge\n";
      return;
    }
    graph[u][v] = w;
    if(!directed)
      graph[v][u] = w;
  }
  void displayGraph();
  void dijkastrasSpt(int source,int* dist,int* parentSet);
};
void Graph::displayGraph() {
  for(int i=0;i<v;++i) {
    for(int j=0;j<v;++j) {
      printf("%-3d ",graph[i][j]);
    }
    cout<<endl;
  }
}
int sum(int d1,int d2) {
  if(d1==INF || d2==INF)
    return INF;

  return d1+d2;
}

void Graph::dijkastrasSpt(int source,int* dist,int* parentSet) {
  bool* spt = new bool[v];
  int findMin(int*,bool*,int);
  for(int i=0;i<v;++i) {
    dist[i] = INF;
    parentSet[i] = -1;
  }

  //include the source vertex
  dist[source] = 0;

  for(int i=0;i<v;++i) {
    int min = findMin(dist,spt,v);
    //printf("min : %d\n",min);
    spt[min] = true;

    //loop to traverse elements adjacent to min
    for(int j=0;j<v;++j) {
      if(!spt[j] && graph[min][j]!=0) {
        int d = sum(dist[min],graph[min][j]); //new distance from source vertex when min in included
        if(d<dist[j]) { //if new distance is less than older update.
          dist[j] = d;
          parentSet[j] = min;
        }
      }
    }
  }
}

int findMin(int* dist,bool* spt,int n) {
  int minDist = INF;
  int min = -1;
  for(int i=0;i<n;++i)
    if(!spt[i] && dist[i]<minDist) {
      min = i;
      minDist = dist[min];
    }

  return min;
}


int main() {
  int n;
  cout<<"Enter the number of vertices\n"<<endl;
  cin>>n;
  Graph graph(n,false);
  int u,v,w;
  cout<<"Enter the Edges in the form (u v w). all the vertices must be connected"<<
        "via some edge\n(Enter -1 to stop)\n";
  while(true) {
    cin>>u;
    if(u==-1)
      break;
    cin>>v;
    cin>>w;
    cin.ignore(numeric_limits<int>::max(),'\n');
    graph.addEdge(u,v,w);
    cout<<"added\n\n";
  }

  /*int n = 9;
  Graph graph(n,false);
  graph.addEdge(0,1,4);
  graph.addEdge(0,7,8);
  graph.addEdge(1,7,11);
  graph.addEdge(1,2,8);
  graph.addEdge(2,8,2);
  graph.addEdge(2,5,4);
  graph.addEdge(2,3,7);
  graph.addEdge(3,4,9);
  graph.addEdge(3,5,14);
  graph.addEdge(4,5,10);
  graph.addEdge(5,6,2);
  graph.addEdge(6,7,1);
  graph.addEdge(6,8,6);
  graph.addEdge(7,8,7);*/

  graph.displayGraph();
  int* parentSet = new int[n];
  int* dist = new int[n];
  graph.dijkastrasSpt(0,dist,parentSet);

  cout<<"Shortest Path Tree\n";
  for(int i=0;i<n;++i) {
    printf("%d -> %d\n",i,parentSet[i]);
  }

  cout<<"Distances from source vertex 0\n";
  for(int i=0;i<n;++i) {
    printf("%d : %d\n",i,dist[i]);
  }
}
