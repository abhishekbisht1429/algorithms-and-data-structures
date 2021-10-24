#include<iostream>
#include<fstream>
#include<chrono>

using namespace std;

#define DEBUG

#define MAX_ROW 15
#define MAX_COL 15

struct node {
    node *map[26];
    bool eow;
    node():eow(false) {
        for(int i=0; i<26; ++i)
            map[i] = nullptr;
    }
};

class trie {
    node *root;

    public:
    trie() {
        root = new node();
    }
    void insert(string str) {
        node *temp = root;
        int i=0;
        while(i<str.size()) {
            int idx = str[i] - 'a';
            if(temp->map[idx]!= nullptr) {
                temp = temp->map[idx];
                ++i;
            } else {
                while(i<str.size()) {
                    idx = str[i] - 'a';
                    temp->map[idx] = new node();
                    temp = temp->map[idx];
                    ++i;
                }
            }
        }
        temp->eow = true;
    }

    node *get_root() {
        return root;
    }
};

template<typename T>
class vector {
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
    vector(size_t size) {
        this->_size = size;
        this->cap = size + size/2;
        container = new T[cap];
    }

    vector() {
        this->cap = 8;
        this->container = new T[cap];
        this->_size = 0;
    }

    /* copy constructor */
    vector(const vector<T> &vec) {
        *this = vec;
    }

    void operator=(const vector<T> &vec) {
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

    ~vector() {
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


char grid[MAX_ROW][MAX_COL];
bool visited[MAX_ROW][MAX_COL];
trie t;
int r, c;

int dir_i[] = {0,  0, 1, -1};
int dir_j[] = {1, -1, 0,  0};

void find(vector<string> &res, string &str, int i, int j, node *nd = t.get_root()) {
    if(i<0 || i == r || j<0 || j == c || visited[i][j])
        return;
    int idx = grid[i][j] - 'a';
    if(nd == nullptr || nd->map[idx] == nullptr)
        return;
    
    /* found a match so print it */
    node *next_node = nd->map[idx];
    if(next_node->eow) {
        str.push_back(grid[i][j]);
        // cout<<str<<" ";
        res.push_back(str);
        str.pop_back();
    }

    visited[i][j] = true;
    for(int x=0; x<4; ++x) {
        str.push_back(grid[i][j]);
        find(res, str, i+dir_i[x], j+dir_j[x], next_node);
        str.pop_back();
    }
    visited[i][j] = false;

    /* check if next_node is leaf */
    bool leaf = true;
    for(int k=0; k<26; ++k) {
        if(next_node->map[k] != nullptr) {
            leaf = false;
            break;
        }
    }
    
    /* delete next_node if it is leaf  */
    if(leaf) {
        delete next_node;
        nd->map[idx] = nullptr;
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

    cin>>r>>c;
    for(int i=0; i<r; ++i)
        for(int j=0; j<c; ++j)
            cin>>grid[i][j];
    
    int x;
    cin>>x;
    for(int i=0; i<x; ++i) {
        string str;
        cin>>str;
        t.insert(str);
    }
    string temp = "";
    vector<string> res;
    for(int i=0; i<r; ++i)
        for(int j=0; j<c; ++j)
            find(res, temp, i, j);

    res.sort();

    for(int i=0; i<res.size(); ++i) {
        cout<<res[i];
        if(i<res.size()-1) cout<<" ";
    }


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