#include<iostream>
#include<fstream>
#include<chrono>
#include<limits>
#include<vector>
#include<iomanip>

using namespace std;

#define DEBUG
#define INF numeric_limits<int>::max()

typedef pair<int, int> _ii;

template<typename T>
class _pq {
    size_t cap;
    size_t _size;
    vector<T> container;

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

void dijkstra_sssp(vector<vector<char>> &map, vector<vector<int>> &dist, int i, int j) {
    int n = map.size();
    int m = map[0].size();
    int dir_i[] = {0,  0, 1, -1};
    int dir_j[] = {1, -1, 0,  0};
    vector<vector<bool>> visited(n, vector<bool>(m, false));
    
    dist[i][j] = 0;

    _pq<pair<int, _ii>> pq(n);
    pq.push({dist[i][j], {i, j}});
    
    /* start main algo */
    while(!pq.empty()) {
        _ii u = pq.front().second;
        int i = u.first;
        int j = u.second;
        pq.pop();
        

        for(int k=0; k<4; ++k) {
            int adj_i = i + dir_i[k];
            int adj_j = j + dir_j[k];
            if( adj_i < 0 || adj_j < 0 || adj_i >= map.size() || adj_j >= map[0].size() ||
                map[adj_i][adj_j] == '#')
                continue;
            
            if(1+dist[i][j] < dist[adj_i][adj_j])
                dist[adj_i][adj_j] = 1 + dist[i][j];

            if(!visited[adj_i][adj_j])
                pq.push({dist[adj_i][adj_j], {adj_i, adj_j}});
        }
        visited[i][j] = true;
    }
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
    vector<vector<char>> map(n, vector<char>(m));
    vector<vector<int>> dist(n, vector<int>(m, INF));
    _ii pos_a;
    vector<_ii> riders;
    map.resize(n, vector<char>(m));
    for(int i=0; i<n; ++i) {
        for(int j=0; j<m; ++j) {
            cin>>map[i][j];
            if(map[i][j] == 'R')
                riders.push_back({i, j});
            else if(map[i][j] == 'A')
                pos_a = {i, j};
        }
    }

    dijkstra_sssp(map, dist, 1, 0);

    bool possible = true;
    for(_ii p : riders) {
        if(dist[p.first][p.second] < dist[pos_a.first][pos_a.second]) {
            possible = false;
            break;
        }
    }

    if(possible)
        cout<<"Yes\n"<<dist[pos_a.first][pos_a.second]<<"\n";
    else
        cout<<"No\n";




    /* #######################CODE_END############################### */
    #ifdef DEBUG
    auto _end = chrono::high_resolution_clock::now();
    auto _duration = chrono::duration_cast<chrono::nanoseconds>(_end - _start);
    long long _ns = _duration.count();
    double _s = _ns / 1e6;
    cout<<"\nDuration: "<<_s<<" ms\n";
    cin.rdbuf(_in_backup);
    cout.rdbuf(_out_backup);
    #endif
    return 0;
}