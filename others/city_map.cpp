#include<iostream>
#include<fstream>
#include<chrono>
#include<limits>
#include<queue>

using namespace std;

#define DEBUG

template<typename T>
class _vec {
    T *container;
    size_t cap;
    size_t _size;

    void grow() {
        size_t new_cap = cap + cap;
        T *new_container = new T[new_cap];
        for(int i=0; i<_size; ++i)
            new_container[i] = container[i];
        
        delete[] container;
        
        container = new_container;
        cap = new_cap;
    }

    int partition(T* arr, int start, int end) {
        T pivot = arr[end];
        int x = start-1;
        for(int i=start;i<end;++i) {
            if(arr[i]<pivot) {
                ++x;
                swap(arr[i], arr[x]); //swap i and x
            }
        }
        swap(arr[end], arr[x+1]); //x+1 is the correct index of pivot
        return x+1;
    }

    void quick_sort(T* arr, int start, int end) {
        if(start>=end)
            return;
        //pivot index
        int pi = partition(arr,start,end);
        quick_sort(arr,start,pi-1);
        quick_sort(arr,pi+1,end);
    }

    public:
    _vec(size_t size) {
        this->_size = size;
        this->cap = size + size/2;
        container = new T[cap];
    }

    _vec(size_t size, T def):_vec(size) {
        for(int i=0; i<size; ++i)
            container[i] = def;
    }

    _vec() {
        this->cap = 8;
        this->container = new T[cap];
        this->_size = 0;
    }

    /* copy constructor */
    _vec(const _vec<T> &vec) {
        *this = vec;
    }

    void operator=(const _vec<T> &vec) {
        this->cap = vec.cap;
        this->container = new T[this->cap];
        this->_size = vec._size;
        for(int i=0; i<_size; ++i)
            this->container[i] = vec.container[i];
    }

    void resize(size_t sz, T def) {
        if(sz <= _size) {
            _size = sz;
        } else {
            while(_size < sz)
                push_back(def);
        }
    }

    ~_vec() {
        delete[] container;
    }

    void push_back(T val) {
        if(_size == cap)
            grow();
        container[_size++] = val;
    }

    void pop_back() {
        _size = min((size_t)0, _size-1);
    }

    T &operator[](int i) {
        return container[i];
    }

    size_t size() {
        return _size;
    }

    T *get_container() {
        return container;
    }

    void sort() {
        quick_sort(container, 0, _size-1);
    }
};

class graph {
    struct edge {
        int v;
        int w;
        edge() {}
        edge(int v, int w):v(v), w(w) {}
    };

    _vec<_vec<edge>> adj_list;
    int n, m;

    public:
    graph(int n): n(n), m(0) {
        adj_list.resize(n, _vec<edge>());
    }

    void add_edge(int u, int v, int w) {
        if(u >= n) {
            n = u+1;
            adj_list.resize(n, _vec<edge>());
        }

        if(v >= n) {
            n = u+1;
            adj_list.resize(n, _vec<edge>());
        }
        adj_list[u].push_back(edge(v, w));
        adj_list[v].push_back(edge(u, w));
        ++m;
    }

    _vec<edge> &operator[](int u) {
        return adj_list[u];
    }

    int size() {
        return m;
    }

    int order() {
        return n;
    }

    void display() {
        for(int i=0; i<n; ++i) {
            cout<<i<<" -> ";
            for(int j=0; j<adj_list[i].size(); ++j) {
                cout<<"{"<<adj_list[i][j].v<<","<<adj_list[i][j].w<<"} ";
            }
            cout<<"\n";
        }
    }

};

_vec<string> dijkstra_sssp(graph &g, int st) {
    int n = g.order();
    // cout<<n<<"\n";
    int inf = numeric_limits<int>::max();
    _vec<bool> visited(n);
    _vec<int> dist(n, inf);
    _vec<int> p(n, -1);

    queue<int> q;
    q.push(st);
    dist[st] = 0;
    while(!q.empty()) {
        int u = q.front();
        q.pop();

        /* relax all edges of u */
        for(int i=0; i<g[u].size(); ++i) {
            int v = g[u][i].v;
            int w = g[u][i].w;

            /* relax edges */
            if(w+dist[u] < dist[v]) {
                dist[v] = w + dist[u];
                p[v] = u;
            } else if(w+dist[u] == dist[v] && u < p[v]) {
                    p[v] = u;
            }
            
            /* push into queue */
            if(!visited[v])
                q.push(v);
        }
        visited[u] = true;
    }

    /* extract path for each node */
    _vec<string> paths;
    for(int i=0; i<n; ++i) {
        int temp = i;
        string path = "";
        while(temp != -1) {
            path += " " + to_string(temp);
            temp = p[temp];
        }
        paths.push_back(path.substr(1));
    }

    return paths;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    #ifdef DEBUG
    ifstream _in("./.test/input.txt");
    ofstream _out("./.test/output.txt");
    streambuf* _in_backup = cin.rdbuf();
    streambuf* _out_backup = cout.rdbuf();
    cin.rdbuf(_in.rdbuf());
    cout.rdbuf(_out.rdbuf());
    auto _start = chrono::high_resolution_clock::now();
    #endif
    /* ######################CODE_START################################ */

    int n, m;
    cin>>n>>m;
    graph g(n);
    for(int i=0; i<m; ++i) {
        int u, v, w;
        cin>>u>>v>>w;
        g.add_edge(u, v, w);
    }
    int dest;
    cin>>dest;
    // g.display();

    _vec<string> paths = dijkstra_sssp(g, dest);

    for(int i=0; i<paths.size(); ++i)
        if(i!=dest) cout<<paths[i]<<"\n";

    /* #######################CODE_END############################### */
    #ifdef DEBUG
    auto _end = chrono::high_resolution_clock::now();
    auto _duration = chrono::duration_cast<chrono::nanoseconds>(_end - _start);
    long long _ns = _duration.count();
    double _s = _ns / 1e9;
    cout<<"\nDuration: "<<_s<<" sec\n";
    cin.rdbuf(_in_backup);
    cout.rdbuf(_out_backup);
    #endif
    return 0;
}