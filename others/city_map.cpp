#include<iostream>
#include<fstream>
#include<chrono>
#include<limits>
#include<stack>
#include<vector>

using namespace std;

#define DEBUG

template<typename T>
class _vec {
    T *container = nullptr;
    size_t cap = 0;
    size_t _size = 0;

    void grow(size_t new_cap) {
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
        container = new T[cap]();
    }

    _vec(size_t size, T def):_vec(size) {
        for(int i=0; i<size; ++i)
            container[i] = def;
    }

    _vec() {
        this->cap = 8;
        this->container = new T[cap]();
        this->_size = 0;
    }

    /* copy constructor */
    _vec(const _vec<T> &vec) {
        this->cap = 8;
        this->container = new T[8]();
        *this = vec;
    }

    void operator=(const _vec<T> &vec) {
        this->_size = 0;
        for(int i=0; i<vec._size; ++i) {
            this->push_back(vec.container[i]);
        }
    }

    ~_vec() {
        delete[] container;
    }

    void resize(size_t sz, T def) {
        if(sz <= _size) {
            _size = sz;
        } else {
            while(_size < sz)
                push_back(def);
        }
    }

    void reserve(size_t cap) {
        if(cap > this->cap)
            grow(cap);
    }

    void push_back(T val) {
        if(_size == cap)
            grow(2 * cap);
        container[_size++] = val;
    }

    void pop_back() {
        _size = max((size_t)0, _size-1);
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

    void display() {
        for(int i=0; i<_size; ++i)
            cout<<container[i]<<" ";
        cout<<"\n";
    }
};

template<typename T>
class _pq {
    size_t cap;
    size_t _size;
    _vec<T> container;

    void heapify_up(size_t node) {
        if(node == 0)
            return;
        
        int p = (node-1)/2;
        if(container[node] < container[p]) {
            swap(container[node], container[p]);
            heapify_up(p);
        }
    }

    void heapify_down(size_t node) {
        if(node >= _size)
            return;
        int l = 2*node+1;
        int r = 2*node+2;
        int c = 0;
        if(l<_size && r<_size) {
            if(container[l] < container[r] && container[l] < container[node])
                c = 1;
            else if(container[r] < container[node])
                c = 2;
        } else if(l < _size && container[l] < container[node]) {
            c = 1;
        } else if(r < _size && container[r] < container[node]) {
            c = 2;
        }

        if(c == 1) {
            swap(container[node], container[l]);
            heapify_down(l);
        } else if(c == 2) {
            swap(container[node], container[r]);
            heapify_down(r);
        }
    }

    public:
    _pq(size_t cap) {
        this->cap = cap;
        container.reserve(cap);
        this->_size = 0;
    }

    void push(T val) {
        if(_size == cap)
            return;
        
        container[_size] = val;
        heapify_up(_size++);
    }

    /* undefined behaviour if size is 0 */
    T front() {
        return container[0];
    }

    void pop() {
        swap(container[0], container[_size-1]);
        --_size;
        heapify_down(0);
    }

    bool empty() {
        return _size == 0;
    }

    size_t size() {
        return _size;
    }
};

class graph {
    struct edge {
        int v;
        int w;
        edge() {}
        edge(int v, int w):v(v), w(w) {}

        bool operator<(edge e) {
            return this->w < e.w;
        }
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
    int inf = numeric_limits<int>::max();
    _vec<bool> visited(n);
    _vec<int> dist(n, inf);
    _vec<int> p(n, -1);
    dist[st] = 0;

    _pq<pair<int, int>> pq(n);
    pq.push({dist[st], st});
    
    /* start main algo */
    while(!pq.empty()) {
        int u = pq.front().second;
        pq.pop();
        
        /* relax all adjacent edges of u */
        for(int i=0; i<g[u].size(); ++i) {
            int v = g[u][i].v;
            int w = g[u][i].w;

            /* relax edge */
            if(w+dist[u] < dist[v]) {
                dist[v] = w + dist[u];
                p[v] = u;
            } else if(w+dist[u] == dist[v] && to_string(u) < to_string(p[v])) {
                    p[v] = u;
            }
            
            /* push into queue */
            if(!visited[v])
                pq.push({dist[v], v});
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

void all_path_from_util(graph &g, _vec<pair<int, string>> &paths, _vec<bool> &visited, int u, _vec<int> &path) {
    if(visited[u])
        return;
    visited[u] = true;
    path.push_back(u);

    /* convert path to string and calculate dist */
    string str = "";
    int dist = 0;
    for(int i=0; i<path.size(); ++i) {
        str += " "+to_string(path[i]);
        
        /* calculate dist */
        if(i+1 < path.size()) {
            int u = path[i];
            /* search in adj list */
            for(int j=0; j<g[path[i]].size(); ++j) {
                int v = g[u][j].v;
                int w = g[u][j].w;
                if(v == path[i+1]) {
                    dist += w;
                    break;
                }
            }
        }
    }
    paths.push_back({dist, str.substr(1)});

    /* visit adjacent nodes */
    for(int i=0; i<g[u].size(); ++i) {
        int v = g[u][i].v;
        all_path_from_util(g, paths, visited, g[u][i].v, path);
    }
    path.pop_back();
    visited[u] = false;
}

_vec<pair<int, string>> all_path_from(graph &g, int u) {
    int n = g.order();
    _vec<pair<int, string>> paths;
    _vec<bool> visited(n, false);
    _vec<int> path;

    all_path_from_util(g, paths, visited, u, path);

    return paths;
}

_vec<string> k_shortest_path(graph &g, int k) {
    int n = g.order();
    _vec<pair<int, string>> total_paths;
    for(int i=0; i<n; ++i) {
        _vec<pair<int, string>> paths = all_path_from(g, i);
        for(int i=0; i<paths.size(); ++i)
            total_paths.push_back(paths[i]);
    }

    _vec<pair<int, string>> total_unique_paths;
    
    /* remove duplicate paths and 0 length paths */
    for(int i=0; i<total_paths.size(); ++i) {
        int d1 = total_paths[i].first;
        string p1 = total_paths[i].second;

        /* exclude 0 length paths */
        if(d1 == 0) continue;

        bool present = false;
        /* check with every path in total_unique_paths */
        for(int j=0; j<total_unique_paths.size(); ++j) {
            int d2 = total_unique_paths[j].first;
            string p2 = total_unique_paths[j].second;
            if(d1 == d2 && p1.size() == p2.size()) {
                present = true;
                for(int k=0; k<p1.size(); ++k) {
                    if(p1[k] != p2[p1.size()-k-1]) {
                        present = false;
                        break;
                    }
                }
            }
            if(present)
                break;
        }
        if(!present)
            total_unique_paths.push_back({d1, p1});
    }
    total_unique_paths.sort();

    _vec<string> res;
    for(int i=0; i<min((int)total_unique_paths.size(), k); ++i)
        res.push_back(total_unique_paths[i].second);

    return res;
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
    // g.display();

    int dest, k;
    cin>>dest>>k;

    /* dijkstra single source shortest path */
    cout<<"dijkstra\n";
    _vec<string> paths = dijkstra_sssp(g, dest);

    for(int i=0; i<paths.size(); ++i)
        if(i!=dest) cout<<paths[i]<<"\n";
    
    /* top k shortest path */
    cout<<"top "<<k<<" shortest paths\n";
    _vec<string> ks = k_shortest_path(g, k);
    cout<<ks.size()<<"\n";
    for(int i=0; i<ks.size(); ++i)
        cout<<ks[i]<<"\n";


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