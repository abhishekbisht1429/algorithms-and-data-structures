#include<iostream>

using namespace std;

template<typename T>
struct deque_iterator {
    T** ptr_node;
    T* _begin;
    T* _end;
    T* curr;
    size_t buf_size;

    deque_iterator(): ptr_node(nullptr), 
                        _begin(nullptr), 
                        _end(nullptr), 
                        curr(nullptr), buf_size(8) {}

    deque_iterator(T** ptr_node, size_t buf_size) {
        this->buf_size = buf_size;
        set_node(ptr_node);
        curr = _begin;
    }

    void set_node(T** new_node) {
        ptr_node = new_node;
        _begin = *ptr_node;
        _end = _begin + buf_size;
        curr = _begin;
    }

    public:
    deque_iterator &operator+=(int n) {
        int offset = curr - _begin + n;
        int node_offset;
        if(offset>=0) {
            node_offset = offset/(int)buf_size;
            offset = offset - node_offset * buf_size;
        } else {
            node_offset = offset/(int)buf_size - 1;
            offset = offset - (node_offset+1)*buf_size;
            offset += buf_size;
        }
        set_node(ptr_node + node_offset);
        curr = _begin + offset;

        return *this;
    }

    deque_iterator &operator-=(int n) {
        return *this += (-n);
    }

    deque_iterator operator+(int n) {
        deque_iterator<T> temp = *this;
        temp += n;
        return temp;
    }

    deque_iterator operator-(int n) {
        deque_iterator<T> temp = *this;
        temp += (-n);
        return temp;
    }

    deque_iterator &operator++() {
        *this += 1;
        return *this;
    }

    deque_iterator &operator--() {
        *this -= 1;
        return *this;
    }

    deque_iterator operator++(int) {
        return ++(*this);
    }

    deque_iterator operator--(int) {
        return --(*this);
    }

    T &operator*() {
        return *curr;
    }

    bool operator==(deque_iterator<T> &it) {
        return ptr_node == it.ptr_node && curr == it.curr;
    }
    bool operator!=(deque_iterator<T> &it) {
        return !(*this == it);
    }
};

template<typename T, size_t buf_size = 8>
class deque {
    allocator<T> dataAllocator;
    allocator<T*> ptrMapAllocator;
    T** ptrMap;
    size_t map_cap;
    deque_iterator<T> _begin;
    deque_iterator<T> _end;

    bool isAtLeftEnd() {
        if(ptrMap == nullptr)
            return true;
        return _begin.ptr_node == ptrMap && 
                _begin.curr == *ptrMap;
    }

    bool isAtRightEnd() {
        if(ptrMap == nullptr)
            return true;
        return _end.ptr_node == (ptrMap + map_cap-1) && 
                _end.curr == *(ptrMap + map_cap - 1) + buf_size - 1;
    }

    void growMap(bool grow_left) {
        /* This function has been made to only increase the size of the map
            the new capacity must be greater than previous 
        */

        /* calculate new map size */
        size_t cap = map_cap * buf_size;
        size_t new_map_cap = (cap + cap/2)/buf_size + 1;

        /* calculate start offset */
        size_t st = grow_left?(new_map_cap - map_cap):(_begin.ptr_node - ptrMap);

        /* allocate new pointer map */
        T** new_ptr_map = ptrMapAllocator.allocate(new_map_cap);
        for(int i=0; i<st; ++i)
            new_ptr_map[i] = dataAllocator.allocate(buf_size);
        for(int i=st+map_cap; i<new_map_cap; ++i)
            new_ptr_map[i] = dataAllocator.allocate(buf_size);

        /* Copy the contents */
        for(int i=st; i<st+map_cap; ++i)
            new_ptr_map[i] = ptrMap[i-st];

        /* update start and _end iterators */
        size_t node_offset = _end.ptr_node - _begin.ptr_node;
        _begin.ptr_node = new_ptr_map + st;
        _end.ptr_node = _begin.ptr_node + node_offset;

        /* deallocate previous ptr map */
        ptrMapAllocator.deallocate(ptrMap, map_cap);
        ptrMap = new_ptr_map;
        map_cap = new_map_cap;
    }

    void deallocateMap(T** ptr, size_t len) {
        for(int i=0; i<len; ++i)
            dataAllocator.deallocate(ptr[i], buf_size);
        ptrMapAllocator.deallocate(ptr, len);
    }

    public:
    deque() {
        map_cap = 0;
        ptrMap = nullptr;
    }

    deque(size_t n, T val) {
        map_cap = n/buf_size + 1;
        ptrMap = ptrMapAllocator.allocate(map_cap);
        for(size_t i=0; i<map_cap; ++i)
            ptrMap[i] = dataAllocator.allocate(buf_size);
        
        _begin.set_node(ptrMap);
        _end.set_node(ptrMap);
        
        for(int i=0; i<n; ++i)
            push_back(val);
    }

    /* Modifier functions */

    void push_back(T val) {
        if(isAtRightEnd())
            growMap(false);
        if(empty()) {
            _begin.set_node(ptrMap);
            _end.set_node(ptrMap);
        }

        *_end = val;
        ++_end;
    }

    void pop_back() {
        if(empty())
            return;
        --_end;
    }

    void push_front(T val) {
        if(isAtLeftEnd())
            growMap(true);
        if(empty()) {
            _begin.set_node(ptrMap);
            _end.set_node(ptrMap);
        }
        --_begin;
        *_begin = val;
    }

    void pop_front() {
        if(empty())
            return;
        ++_begin;
    }

    void resize(size_t n, T def) {
        size_t current_size = size();
        if(n < current_size) {
            for(int i=0; i<current_size - n; ++i)
                pop_back();
        } else {
            for(int i=0; i<n - current_size; ++i)
                push_back(def);
        }
    }

    void clear() {
        deallocateMap(ptrMap, map_cap);
        ptrMap = nullptr;
        _begin.ptr_node = nullptr;
        _end.ptr_node = nullptr;

        _begin._begin = nullptr;
        _begin._end = nullptr;
        _begin.curr = nullptr;

        _end._begin = nullptr;
        _end._end = nullptr;
        _end.curr = nullptr;
        map_cap = 0;
    }

    /* access functions */

    T front() {
        return *_begin;
    }

    T back() {
        return *(_end-1);
    }

    bool empty() {
        return _begin == _end;
    }

    size_t size() {
        size_t size = (_end.ptr_node - _begin.ptr_node+1)*buf_size;
        size -= (_begin.curr - _begin._begin);
        size -= buf_size - (_end.curr - _end._begin);
        return size;
    }

    T operator[](size_t pos) {
        return *(_begin+pos);
    }

    deque_iterator<T> begin() {
        return _begin;
    }

    deque_iterator<T> end() {
        return _end;
    }

    void print() {
        if(empty())
            return;
        for(deque_iterator<T> it = _begin; it!=_end; ++it)
            cout<<*it<<" ";
        cout<<"\n";
    }
};

int main() {
    cout<<"testing deque_iterator implementation\n";
    int **ptrMap = allocator<int*>().allocate(8);
    for(int i=0; i<8; ++i) {
        ptrMap[i] = allocator<int>().allocate(8);
        for(int j=0; j<8; ++j)
            ptrMap[i][j] = 8*i + j;
    }
    for(int i=0; i<8; ++i)
        for(int j=0; j<8; ++j)
            cout<<ptrMap[i][j]<<" ";
    cout<<"\n";

    deque_iterator<int> itr(ptrMap, 8);
    cout<<*(itr+15)<<"\n";

    itr += 20;
    cout<<*itr<<"\n";
    cout<<*(itr-10)<<"\n";
    cout<<*(itr+32)<<"\n";

    itr += 32;
    cout<<*(itr-29)<<"\n";
    cout<<*(itr+11)<<"\n";

    deque_iterator<int> _begin(ptrMap, 8), _end = _begin + 64;

    for(deque_iterator<int> it = _begin; it!=_end; ++it)
        cout<<*it<<" ";
    cout<<"\n";

    cout<<"Testing push back\n";
    deque<int> dq;
    cout<<dq.size()<<"\n";
    cout<<dq.empty()<<"\n";
 

    dq.push_back(34);
    dq.push_back(22);
    dq.push_back(35);
    dq.print();

    cout<<"Testing push front\n";
    dq.push_front(22);
    dq.push_front(1);
    dq.push_front(-1);
    dq.print();

    cout<<dq.size()<<"\n";

    cout<<"Testing front and back\n";
    cout<<dq.front()<<"\n";
    cout<<dq.back()<<"\n";

    cout<<"Testing random access\n";
    cout<<dq[0]<<"\n";
    cout<<dq[dq.size()-1]<<"\n";

    cout<<"Testing pop_back\n";
    dq.pop_back();
    dq.print();
    dq.pop_back();
    dq.print();
    dq.pop_back();
    dq.print();
    dq.pop_back();
    dq.print();
    dq.pop_back();
    dq.print();
    dq.pop_back();
    dq.print();

    dq.push_back(-1);
    dq.push_back(2);
    dq.push_back(3);
    dq.push_front(-3);
    dq.print();

    cout<<"Testing pop_front\n";
    dq.pop_front();
    dq.print();
    dq.pop_front();
    dq.print();
    dq.pop_front();
    dq.print();
    dq.pop_front();
    dq.print();

    cout<<"Testing clear\n";
    dq.clear();
    dq.print();
    dq.push_back(1);
    dq.push_front(2);
    dq.print();

    cout<<"Testing resize\n";
    cout<<dq.size()<<"\n";
    dq.print();
    dq.resize(10, 100);
    dq.print();
    dq.resize(4, 100);
    dq.print();
    cout<<dq.empty()<<"\n";

    cout<<"Testing dequeu 2nd constructor\n";
    deque<string> dq2(5, "hello");
    dq2.print();
    dq2.push_back("world!");
    dq2.push_front("hello!");
    cout<<dq2.front()<<"\n";
    cout<<dq2.back()<<"\n";
    dq2.print();
    dq2.clear();
    cout<<dq2.size()<<"\n";
    dq2.print();
}