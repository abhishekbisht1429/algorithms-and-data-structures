#include<iostream>
#include<vector>
#include<limits>
#define INF numeric_limits<int>::max()

using namespace std;
struct Graph {
  vector<pair<pair<int,int>,int>>* edgeList;
  int v;
  Graph(int v) {
    this->v = v;
    edgeList = new vector<pair<pair<int,int>,int>>();
  }

  void addEdge(int u,int v,int w) {
    if(u>=this->v || v>=this->v) {
      cout<<"invalid edge\n";
      return;
    }

    edgeList->push_back({{u,v},w});
  }

  void displayGraph();
  bool bellmanfordSpt(int source,int* parentSe,int* dist);
};


void Graph::displayGraph() {
  vector<pair<pair<int,int>,int>>::iterator itr;
  for(itr = edgeList->begin();itr!=edgeList->end();++itr) {
    printf("%d -> %d\n",(*itr).first.first,(*itr).first.second);
  }
  cout<<endl;
}
int sum(int d1,int d2) {
  if(d1==INF || d2==INF)
    return INF;

  return d1+d2;
}

bool Graph::bellmanfordSpt(int source,int* parentSet,int* dist) {
  int sum(int,int); // utitlity function to add distances
  for(int i=0;i<v;++i) {
    dist[i] = i==source?0:INF;
    parentSet[i] = -1;
  }

  /*below, i represents the path length from source vertex.
    Path length for vertex v is the minimum no. of edges required to reach v from source.
    A graph with v edges, cannot have its path length greater than v-1
  */
  for(int i=1;i<v;++i) { // start from path length 1 and iterate upto v-1
    vector<pair<pair<int,int>,int>>::iterator itr;
    for(itr = edgeList->begin();itr!=edgeList->end();++itr) { //relax each edge
      int u = (*itr).first.first;//source of current edge
      int v = (*itr).first.second;//dest of current edge
      int w = (*itr).second;//weight of current edge
      //relaxation procedure begin
      if(dist[v]>sum(dist[u],w)) {
        dist[v] = sum(dist[u],w);
        parentSet[v] = u;
      }//relaxation procedure end
    }
  }

  //check for negetive weight cycle
  vector<pair<pair<int,int>,int>>::iterator itr;
  for(itr = edgeList->begin();itr!=edgeList->end();++itr) {
    int u = (*itr).first.first;//source of current edge
    int v = (*itr).first.second;//dest of current edge
    int w = (*itr).second;//weight of current edge
    //if we still get a shorter path
    //it means negetive edge cycle exists
    if(sum(dist[u],w)<dist[v])
      return false;
  }

  return true;
}

int main() {
  int n;
  cout<<"Enter the number of vertices\n"<<endl;
  cin>>n;
  Graph graph(n);
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

  /*Graph graph(v);
  graph.addEdge(0,1,-1);
  graph.addEdge(0,2,4);
  graph.addEdge(1,2,3);
  graph.addEdge(1,3,2);
  graph.addEdge(1,4,2);
  graph.addEdge(3,1,1);
  graph.addEdge(3,2,5);
  graph.addEdge(4,3,-3);
  */
  graph.displayGraph();
  int* parentSet = new int[n];
  int* dist = new int[n];
  if(graph.bellmanfordSpt(0,parentSet,dist)) {
    cout<<"Shortest Path Tree\n";
    for(int i=0;i<n;++i) {
      printf("%d -> %d\n",i,parentSet[i]);
    }
    cout<<"Distances from source vertex 0\n";
    for(int i=0;i<n;++i) {
      printf("%d : %d\n",i,dist[i]);
    }
  } else {
    cout<<"Negetive weight cycle detected"<<endl;
  }
}
