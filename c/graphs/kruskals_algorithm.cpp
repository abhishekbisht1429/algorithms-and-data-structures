#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

typedef struct Graph {
  vector<pair<pair<int,int>,int>> *edges;
  Graph() {
    edges = new vector<pair<pair<int,int>,int>>();
  }

  void addEdge(int u,int v,int w) {
    edges->push_back({{u,v},w});
  }

  void displayGraph() {
    vector<pair<pair<int,int>,int>>::iterator itr;
    for(itr = edges->begin();itr!=edges->end();++itr) {
      printf("%d -> %d\n",(*itr).first.first,(*itr).first.second);
    }
    cout<<endl;
  }

  void kruskalsMst(vector<pair<pair<int,int>,int>>* solVec);
} Graph;

typedef struct Sets {
  vector<int>** sets;
  bool* discarded;
  int v;
  Sets(int v) {
    this->v = v;
    sets = new vector<int>*[v];
    discarded = new bool[v];
    for(int i=0;i<v;++i) {
      sets[i] = new vector<int>();
      //first element of each set is the representative element of that set
      sets[i]->push_back(i);
    }
  }

  vector<int>* findSet(int u);

  vector<int>* mergeSets(vector<int>* set1,vector<int>* set2);
} Sets;

vector<int>* Sets::findSet(int u) {
  for(int i=0;i<v;++i) {
    if(!discarded[i]) {
      vector<int>::iterator itr;
      for(itr = sets[i]->begin();itr!=sets[i]->end();++itr) {
        if(*itr == u)
          return sets[i]; //return the pointer to the set
      }
    }
  }
  return NULL;
}
vector<int>* Sets::mergeSets(vector<int>* set1,vector<int>* set2) {
  vector<int>* discaredSet = (set1->front()>set2->front())?set1:set2;
  vector<int>* mergedSet = (set1->front()<set2->front())?set1:set2;
  vector<int>::iterator itr;
  for(itr = discaredSet->begin();itr!=discaredSet->end();++itr) {
    mergedSet->push_back(*itr);
  }
  discarded[discaredSet->front()] = true;

  return mergedSet;
}
bool compare(pair<pair<int,int>,int> edge1,pair<pair<int,int>,int> edge2) {
  return edge1.second<edge2.second;
}
void Graph::kruskalsMst(vector<pair<pair<int,int>,int>>* solVec) {
  Sets sets(edges->size()); //make-set process implicitly done
  sort(edges->begin(),edges->end(),compare); //sort the edges in ascending order

  vector<pair<pair<int,int>,int>>::iterator itr;
  for(itr = edges->begin();itr!=edges->end();++itr) {
    int u = (*itr).first.first;
    int v = (*itr).first.second;

    vector<int>* setOfu = sets.findSet(u);
    vector<int>* setOfv = sets.findSet(v);
    if(setOfu!=setOfv) { // if the sets of u and v are disjoint then include the edge
                        // (u,v) in the solution vector else ignore it.
      solVec->push_back(*itr);//add the edge to the solution vector;
      sets.mergeSets(setOfu,setOfv);
    }
  }
}

int main() {
  Graph graph;
  /*graph.addEdge(0,1,8);
  graph.addEdge(0,2,2);
  graph.addEdge(0,3,14);
  graph.addEdge(1,3,21);
  graph.addEdge(2,3,25);
  graph.addEdge(2,4,19);
  graph.addEdge(3,4,17);
  graph.addEdge(3,5,13);
  graph.addEdge(4,5,5);
  graph.addEdge(4,6,9);
  graph.addEdge(5,6,1);
  graph.displayGraph(); */
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

  vector<pair<pair<int,int>,int>>* solVec = new vector<pair<pair<int,int>,int>>();
  graph.kruskalsMst(solVec);

  cout<<"MST using Kruskal \n";
  vector<pair<pair<int,int>,int>>::iterator itr;
  int totalCost = 0;
  for(itr = solVec->begin();itr!=solVec->end();++itr) {
    int u = (*itr).first.first;
    int v = (*itr).first.second;
    int w = (*itr).second;
    printf("%d -> %d   %d\n",u,v,w);
    totalCost += w;
  }

  cout<<"Total Cost : "<<totalCost<<endl;
}
