#include<iostream>
#include<list>

using namespace std;

//structre to represent Graph
typedef struct Graph {
  int v;
  list<int>* adj;

  Graph(int v) {
    this->v = v;
    adj = new list<int>[v];
  }

  //function to add new edge
  void addEdge(int u,int v) {
    if(u>=this->v || v>=this->v) {
      cout<<"Invalid Edge"<<endl;
    }
    adj[u].push_back(v);
  }

  void dfs_traversal(int source,list<int>* solVec);
  void display_graph();
} Graph;

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

void Graph::dfs_traversal(int source,list<int>* solVec) {
  //bool array to store the visited nodes
  bool* visited = new bool[v];
  for(int i=0;i<v;++i) {
    visited[i] = false;
  }

  list<int> stack;
  //add source to the stack
  stack.push_front(source);
  visited[source] = true;

  //travrse the graph
  while(!stack.empty()) {
    //pop the first node from the stack
    int current = stack.front();
    stack.pop_front();

    /*add the nodes which are not visited and are adjacent to
      the popped node to the stack */
    list<int>::iterator itr;
    for(itr = adj[current].begin();itr!=adj[current].end();++itr) {
      if(!visited[*itr]) {
        visited[*itr] = true;
        stack.push_front(*itr);
      }
    }
    //add the traversed node to the solution vector
    solVec->push_back(current);
  }
}

//main program for the usage
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

  cout<<"DFS Traversal of graph"<<endl;
  list<int>* solVec = new list<int>();
  g.dfs_traversal(0,solVec);
  list<int>::iterator i;
  for(i = solVec->begin();i!=solVec->end();++i) {
    cout<<*i<<" ";
  }
  cout<<endl;
}
