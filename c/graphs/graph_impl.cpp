#include<iostream>
#include<vector>
#include<utility>

using namespace std;

struct Graph {
  int v;// v is the number of vertices
  vector<pair<int,pair<int,int>>> edges; // edges holds the edges and their weights
  Graph(int v) {
    this->v = v;
  }

  void addEdge(int u,int v,int w) {
    if(u>=this->v || v>=this->v) {
      printf("Operation Not Possible!\n");
      return;
    }
    edges.push_back({w,{u,v}});
  }

  // This function will display the graph in the form of Adjacency Matrix
  void displayGraph();
};

void Graph::displayGraph() {
  int** graphMatrix = new int*[v];
  for(int i=0;i<v;i++) {
    graphMatrix[i] = new int[v];
  }
  for(int i=0;i<edges.size();i++) {
    pair<int,pair<int,int>> edgePair = edges.at(i);
    pair<int,int> edge = get<1>(edgePair);
    graphMatrix[get<0>(edge)][get<1>(edge)] = get<0>(edgePair);
  }
  for(int i=0;i<v;++i) {
    for(int j=0;j<v;++j) {
      printf("%-4d ",graphMatrix[i][j]);
    }
    cout<<endl;
  }
}

int main() {
  Graph graph(8);
  graph.addEdge(0,1,14);
  graph.addEdge(0,2,6);
  graph.addEdge(0,4,5);
  graph.addEdge(0,3,10);
  graph.addEdge(1,3,3);
  graph.addEdge(2,4,4);
  graph.addEdge(3,5,8);
  graph.addEdge(4,5,2);
  graph.addEdge(4,6,9);
  graph.addEdge(5,7,15);

  //Adding the edge below will be invalid so addEdge will print "Operation not possible!"
  graph.addEdge(34,34,23);
  graph.displayGraph();
}
