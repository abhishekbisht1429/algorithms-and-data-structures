#include<iostream>
#include<list>

using namespace std;

//strucutre to represent Graph
typedef struct Graph {
  list<int> *adj;
  int v;

  Graph(int v) {
    this->v = v;
    adj = new list<int>[v];
  }

  void addEdge(int u,int v) { // u - source and v - destination;
    if(u>=this->v || v>=this->v) {
      cout<<"Out of Bounds!"<<endl;
      return;
    }
    adj[u].push_back(v);
  }

  void bfs(int source,list<int>* solVec);
  void display_graph();
} Graph;

void Graph::bfs(int source,list<int>* solVec) {
  list<int> queue;
  //bool array to store visited nodes
  bool* visited = new bool[v];
  for(int i=0;i<v;++i)
    visited[i] = false;

  //add source element to the queue
  queue.push_back(source);
  visited[source] = true;
  while(!queue.empty()) {
    //remove the first node from the queue
    int current = queue.front();
    queue.pop_front();

    /*Add those nodes which are adjacent to the removed node and are not visited, to
      the stack */
    list<int>::iterator i;
    for(i = adj[current].begin();i!=adj[current].end();++i) {
      if(!visited[*i]) {
        visited[*i] = true;
        queue.push_back(*i);
      }
    }
    solVec->push_back(current);
  }
}

//Function to display the Adjacency list - though it is not necessary for traversal
void Graph::display_graph() {
  for(int i=0;i<v;++i) {
    cout<<i<<"->";
    list<int>::iterator itr;
    for(itr = adj[i].begin();itr != adj[i].end();++itr) {
      cout<<*itr<<" ";
    }
    cout<<endl;
  }
}

//main function for the usage
int main() {
  Graph g(7);
  g.addEdge(0,1);
  g.addEdge(0,3);
  g.addEdge(1,4);
  g.addEdge(1,2);
  g.addEdge(2,4);
  g.addEdge(2,5);
  g.addEdge(2,3);
  g.addEdge(3,6);
  g.addEdge(4,5);
  g.addEdge(5,6);

  cout<<"Adjacency List Representation of Graph"<<endl;
  g.display_graph();

  cout<<"BFS Traversal of graph"<<endl;
  list<int>* solVec = new list<int>();
  g.bfs(0,solVec);
  list<int>::iterator i;
  for(i = solVec->begin();i!=solVec->end();++i) {
    cout<<*i<<" ";
  }
  cout<<endl;
}
