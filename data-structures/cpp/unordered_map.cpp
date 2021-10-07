#include<iostream>

using namespace std;

template<typename T> struct default_value;
template<> struct default_value<int> { static constexpr int value = 0; };
template<> struct default_value<char> { static constexpr char value = 0; };
template<> struct default_value<short> { static constexpr short value = 0; };
template<> struct default_value<float> { static constexpr float value = 0.0f; };
template<> struct default_value<double> { static constexpr double value = 0.0; };
template<> struct default_value<long long> { static constexpr long long value = 0l; };
template<> struct default_value<string> { static constexpr const char *value = ""; };

/* 
    MurmurHash3
*/
uint32_t fmix32(uint32_t h) {
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;

  return h;
}

inline uint32_t rotl32(uint32_t val, uint8_t n) {
    return (val<<n) | (val>>(32-n)); 
}

inline uint32_t getblock32(const uint32_t *blocks, int i) {
    return blocks[i];
}

uint32_t murmurHash3_32(const void *key, int len, uint32_t seed) {
    /* 
        len = number of 8 bit blocks in key
    */
    const uint8_t *data = (const uint8_t*)key;
    const int nblocks = len / 4;
    uint32_t h1 = seed;

    /*
        constants specific to the algorithm
    */
    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593; 

    /*
        body
    */
    const uint32_t *blocks = (const uint32_t *)(data + nblocks*4);  
    for(int i = -nblocks; i; i++) {
        /*
            loop over the 32 bit blocks
        */
        uint32_t k1 = getblock32(blocks, i);  
        k1 *= c1;
        k1 = rotl32(k1, 15);
        k1 *= c2;

        h1 ^= k1;
        h1 = rotl32(h1, 13); 
        h1 = h1*5+0xe6546b64;
    }

    /*
        tail - handle the remaining bits
    */
    const uint8_t * tail = (const uint8_t*)(data + nblocks*4);  
    uint32_t k1 = 0;    
    switch(len & 3) {
        case 3: k1 ^= tail[2] << 16;
        case 2: k1 ^= tail[1] << 8;
        case 1: k1 ^= tail[0];
                k1 *= c1; 
                k1 = rotl32(k1,15); 
                k1 *= c2; h1 ^= k1;
    };  

    /*
        Finalize
    */
    h1 ^= len;  
    h1 = fmix32(h1);

    return h1;
} 

/* 
    Exception classes 
*/
class no_key_exception : public exception {
    const char *what() const throw() {
        return "invalid call to the function - no corresponding key in the list";
    }
};

class no_hash_function_defined : public exception {
    const char *what() const throw() {
        return "No hash function defined for the type";
    }
};

/* 
    base class for hash function objects
*/
class _hash_base {
    uint32_t _len;
    uint32_t _seed;
    
    public:
    _hash_base():_len(0), _seed(0) {}
    
    void set_len(uint32_t len) {
        this->_len = len;
    }

    void set_seed(uint32_t seed) {
        this->_seed = seed;
    }

    uint32_t seed() {
        return _seed;
    }

    uint32_t len() {
        return _len;
    }
};

/*
    generic hash function object
*/
template<typename T> class _hash {
    public:
    uint32_t operator()(T key) {
        throw no_hash_function_defined();
    }
};

/* 
    hash function object int specialization
*/
template<> class _hash<int> : public _hash_base {
    public:
    uint32_t operator()(int key) {
        return murmurHash3_32(&key, sizeof(int), seed()) & (len()-1);
    }
};

/* 
    hash function object char specialization
*/
template<> class _hash<char> : public _hash_base {   
    public:
    uint32_t operator()(char key) {
        return murmurHash3_32(&key, sizeof(char), seed()) & (len()-1);
    }
};

/*
    hash function object short specialization
*/
template<> class _hash<short> : public _hash_base {
    public:
    uint32_t operator()(short key) {
        return murmurHash3_32(&key, sizeof(short), seed()) & (len()-1);
    }
};

/*
    hash function object string specialization
*/
template<> class _hash<string> : public _hash_base {
    public:
    uint32_t operator()(string str) {
        const char *key = str.data();
        return murmurHash3_32(key, sizeof(char), seed()) & (len()-1);
    }
};

/*
    hash function object float specialization
*/
template<> class _hash<float> : public _hash_base{
    public:
    uint32_t operator()(float f) {
        union {
            float x;
            unsigned int u;
        };
        x = f;

        return murmurHash3_32(&u, sizeof(unsigned), seed()) & (len()-1);
    }
};

/*
    hash function object double specialization
*/
template<> class _hash<double> : public _hash_base {
    public:
    uint32_t operator()(double d) {
        union {
            double x;
            unsigned long long u;
        };
        x = d;

        return murmurHash3_32(&u, sizeof(unsigned long long), seed()) & (len()-1);
    }
};

/*
    bucket node to store values
*/
template<typename K, typename V> struct node {
    node<K, V> *prev;
    node<K, V> *next;
    pair<K, V> data;
    node(pair<K, V> data): prev(nullptr), next(nullptr), data(data) {}
};

/* 
    bucket implemented as a doubly linked list
*/
template<typename K, typename V> class bucket {
    node<K, V> *_front;
    node<K, V> *_back;
    int _size;


    private:node<K, V> *detach(node<K, V> *nd) {
        if(nd == nullptr)
            return nullptr;
        
        node<K, V> *prev = nd->prev;
        node<K, V> *next = nd->next;

        if(prev!=nullptr) prev->next = next;
        else _front = next;
        if(next!=nullptr) next->prev = prev;
        else _back = prev;

        nd->next = nullptr;
        nd->prev = nullptr;
        --_size;

        return nd;
    }

    private:void insertAfter(node<K, V> *nd, node<K, V> *new_node) {
        if(nd == nullptr || new_node == nullptr)
            return;
        ++_size;
        new_node->next = nd->next;
        new_node->prev = nd;
        if(nd->next != nullptr) nd->next->prev = new_node;
        nd->next = new_node;

        if(new_node->next == nullptr)
            _back = new_node;
    }

    private:void insertBefore(node<K, V> *nd, node<K, V> *new_node) {
        if(nd == nullptr || new_node == nullptr)
            return;
        ++_size;
        new_node->next = nd;
        new_node->prev = nd->prev;
        if(nd->prev != nullptr) nd->prev->next = new_node;
        nd->prev = new_node;

        if(new_node->prev == nullptr)
            _front = new_node;
    }

    public:bucket(): _front(nullptr), _back(nullptr), _size(0) {}

    ~bucket() {
        purge();
    }

    public:void push_back(pair<K, V> data) {
        node<K, V> *nd = new node<K, V>(data);
        if(empty()) {
            _size++;
            _back = nd;
            _front = nd;
            return;
        }

        insertAfter(_back, nd);
    }

    public:void push_front(pair<K, V> data) {
        node<K, V> *nd = new node<K, V>(data);
        if(empty()) {
            _size++;
            _back = nd;
            _front = nd;
            return;
        }

        insertBefore(_front, nd);
    }

    public:void pop_front() {
        delete detach(_front);
    }

    public:void pop_back() {
        delete detach(_back);
    }

    public:bool empty() {
        return _back == nullptr;
    }

    public:int size() {
        return _size;
    }

    public:pair<K, V> front() {
        return _front->data;
    }

    public:pair<K, V> back() {
        return _back->data;
    }

    public:pair<K, V> search(K key, V def = default_value<V>::value) {
        node<K, V> *temp = _front;
        while(temp!=nullptr) {
            if(temp->data.first == key)
                return temp->data;
            temp = temp->next;
        }
        return {key, def};
    }

    bool contains(K key) {
        node<K, V> *temp = _front;
        while(temp!=nullptr) {
            if(temp->data.first == key)
                return true;
            temp = temp->next;
        }
        return false;
    }

    void update(K key, V val) {
        node<K, V> *temp = _front;
        while(temp!=nullptr) {
            if(temp->data.first == key) {
                temp->data.second = val;
                return;
            }
        }
    }

    /* This must be called only after ensuring that the 
        corresponding key exists
    */
    V &get_ref(K key) {
        node<K, V> *temp = _front;
        while(temp!=nullptr && temp->data.first != key) {
            // cout<<"temp->data.first "<<temp->data.first<<"\n";
            temp = temp->next;
        }
        if(temp == nullptr)
            throw no_key_exception();
        return temp->data.second;
    }

    public:void erase(K key) {
        node<K, V> *temp = _front;
        while(temp!=nullptr && temp->data.first != key)
            temp = temp->next;
        if(temp != nullptr)
            delete detach(temp);
    }

    public:void purge() {
        while(!empty())
            pop_back();
    }
};

/*
    unordered map implementation
*/
template<typename K, typename V, typename Hash = _hash<K>>
class unordered_map {
    bucket<K, V> *container;
    size_t _size;
    size_t _bc;
    _hash<K> hash;
    float _max_load_factor = 1.0f;
    allocator<bucket<K, V>> bucketAllocator;

    float load_factor() {
        return (float)_size / (float)_bc;
    }

    float max_load_factor() {
        return _max_load_factor;
    }

    /* call this function everytime the container size is increased */
    void rehash() {
        cout<<"rehashing\n";
        size_t new_bc = 2 * _bc;
        bucket<K, V> *new_container = bucketAllocator.allocate(new_bc);

        /* TODO: set new hash parameters here */
        hash.set_len(new_bc);

        /* copy the contents to new container */
        for(size_t i = 0; i<_bc; ++i) {
            while(!container[i].empty()) {
                K key = container[i].front().first;
                V val = container[i].front().second;
                container[i].pop_front();

                // cout<<key<<" "<<val<<" "<<hash(key)<<"\n";
                new_container[hash(key)].push_front({key, val});
            }
        }

        /* deallocate memory for old container */
        bucketAllocator.deallocate(container, _bc);

        /* update the class variables */
        _bc = new_bc;
        container = new_container;
    }

    public:
    unordered_map() {
        container = bucketAllocator.allocate(2);
        _bc = 2;
        _size = 0;
        hash.set_len(_bc);
    }
    ~unordered_map() {
        bucketAllocator.deallocate(container, _bc);
    }

    void insert(K key, V val) {
        if(load_factor() >= max_load_factor())
            rehash();
        uint32_t h = hash(key);
        if(container[h].contains(key)) {
            container[h].update(key, val);
        } else {
            container[h].push_front({key, val});
            ++_size;
        }
        // cout<<"inserted "<<key<<"\n";
    }

    void erase(K key) {
        container[hash(key)].erase(key);
        --_size;
    }

    bool find(K key) {
        uint32_t h = hash(key);
        // cout<<"find "<<key<<" "<<h<<"\n";
        if(container[h].size() == 0)
            return false;
        
        return container[h].contains(key);
    }

    V &operator[](K key) {
        V def = default_value<V>::value;
        if(!find(key))
            insert(key, def);
        uint32_t h = hash(key);
        // cout<<"[ ] "<<key<<" "<<h<<"\n";
        return container[h].get_ref(key);
    }

    size_t size() {
        return _size;
    }

    size_t bucket_count() {
        return _bc;
    }

};

int main() {
    unordered_map<int, int> umap;
    umap.insert(1, 2);

    cout<<umap[1]<<"\n";
    cout<<umap.find(2)<<"\n";
    cout<<umap[2]<<"\n";
    cout<<umap.find(2)<<"\n";

    unordered_map<int, int> freq_map;
    int arr[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 4, 5, 7, 343, 343, 545, 345, 134, 566, 45};

    cout<<"bucket count "<<freq_map.bucket_count()<<"\n";
    for(int i=0; i<sizeof(arr)/sizeof(arr[0]); ++i) {
        freq_map[arr[i]] = freq_map[arr[i]] + 1;
    }
    cout<<"bucket count "<<freq_map.bucket_count()<<"\n";

    cout<<"Testing find\n";
    cout<<freq_map.find(34)<<"\n";
    cout<<freq_map.find(3)<<"\n";
    cout<<freq_map.find(343)<<"\n";
    cout<<freq_map.find(45)<<"\n";
    cout<<freq_map.find(1)<<"\n";
    cout<<freq_map.find(340)<<"\n";

    cout<<"Testing erase\n";
    freq_map.erase(1);
    cout<<freq_map.size()<<"\n";
    cout<<freq_map.find(1)<<"\n";
    freq_map.erase(2);
    cout<<freq_map.size()<<"\n";
    cout<<freq_map.find(1)<<"\n";
    freq_map.erase(343);
    cout<<freq_map.size()<<"\n";
    cout<<freq_map.find(1)<<"\n";
    freq_map.erase(45);
    cout<<freq_map.size()<<"\n";
    cout<<freq_map.find(1)<<"\n";
    freq_map.erase(1);
    cout<<freq_map.size()<<"\n";
    cout<<freq_map.find(1)<<"\n";
    cout<<freq_map.find(545)<<"\n";

    cout<<"Testing murmur hash\n";
    int keys[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    for(int i=0; i<sizeof(keys)/sizeof(keys[0]); ++i)
        cout<<((murmurHash3_32(keys+i, 4, 0))&(1024-1))<<"\n";

    cout<<"\nTesting umap with string key"<<"\n";
    unordered_map<string, int> umapS;
    umapS.insert("one", 1);
    umapS.insert("two", 2);
    cout<<umapS.find("one")<<"\n";
    umapS.insert("three", 3);
    cout<<umapS.find("one")<<"\n";

    cout<<umapS.size()<<"\n";
    cout<<umapS.bucket_count()<<"\n";

    cout<<umapS["one"]<<"\n";
    cout<<umapS["two"]<<"\n";
    cout<<umapS["three"]<<"\n";

    cout<<"\nTesting umap with float\n";
    unordered_map<float, string> umapF;
    umapF.insert(1.01f, "one");
    umapF.insert(2.0f, "two");
    umapF.insert(3.0f, "three");
    cout<<umapF.size()<<"\n";
    cout<<umapF.bucket_count()<<"\n";

    cout<<umapF[1.01f]<<"\n";
    cout<<umapF[3.0f]<<"\n";

    cout<<"\nTesting umap with double\n";
    unordered_map<double, string> umapD;
    umapD.insert(1.23234, "value 1");
    umapD.insert(1.35556, "value 2");
    umapD.insert(3.2343, "value 3");
    umapD.insert(45.354345, "value 4");
    umapD.insert(422.345, "value 5");
    cout<<umapD.size()<<"\n";
    cout<<umapD.bucket_count()<<"\n";

    cout<<umapD[1.23234]<<"\n";
    cout<<umapD[422.3450]<<"\n";

    cout<<"\nTesting umap with char\n";
    unordered_map<char, int> umapC;
    umapC['A'] = 1;
    umapC['B'] = 2;
    umapC['C'] = 3;
    umapC['D'] = 4;
    umapC['E'] = 5;
    cout<<"size "<<umapC.size()<<"\n";
    cout<<"bucket count "<<umapC.bucket_count()<<"\n";

    cout<<umapC['B']<<"\n";
    cout<<umapC['C']<<"\n";
    cout<<umapC['E']<<"\n";

    cout<<"\nTesting umap with short\n";
    unordered_map<short, string> umapSh;
    umapSh[1] = "value 1";
    umapSh[2] = "value 2";
    umapSh[3] = "value 3";
    umapSh[4] = "value 4";
    umapSh[4] = "value 5";
    cout<<"size "<<umapSh.size()<<"\n";
    cout<<"bucket count "<<umapSh.bucket_count()<<"\n";

    cout<<umapSh[1]<<"\n";
    cout<<umapSh[2]<<"\n";
    cout<<umapSh[4]<<"\n";
    return 0;
}