#include<iostream>
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<algorithm>
#include<chrono>
#include<fstream>

#define DEBUG 

using namespace std;

struct node {
    unordered_map<char, node*> umap;
    bool eow;
    node(): eow(false) {}
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
            if(temp->umap.find(str[i]) != temp->umap.end()) {
                temp = temp->umap[str[i++]];
            } else {
                while(i<str.size()) {
                    temp->umap[str[i]] = new node();
                    temp = temp->umap[str[i++]];
                }
            }
        }
        temp->eow = true;
    }

    bool find(string str) {
        node *temp = root;
        for(int i=0; i<str.size(); ++i) {
            if(temp->umap.find(str[i]) == temp->umap.end())
                return false;
            temp = temp->umap[str[i]];
        }
        return temp->eow;
    }

    void all(vector<string> &vec, node *nd, string s = "") {
        if(nd == nullptr)
            return;
        
        if(nd->eow)
            vec.push_back(s);

        for(auto &p : nd->umap)
            all(vec, p.second, s+p.first);

    }

    vector<string> auto_complete(string str) {
        vector<string> res;
        node *temp = root;
        int i = 0;
        while(i<str.size() && temp->umap.find(str[i]) != temp->umap.end())
            temp = temp->umap[str[i++]];
        
        if(i == str.size())
            all(res, temp, str);
        
        sort(res.begin(), res.end());

        return res;
    }
    
    void auto_correct_util(node *nd, unordered_set<string> &res, string &inp, string str = "", int i=0, int d=0) {
        if(d > 3)
            return;
        
        if(nd->eow) {
            int rem = max(0, ((int)inp.size()) - i);
            // cout<<str<<" "<<rem + d<<"\n";
            if(rem + d <= 3)
                res.insert(str);
        }
        for(auto &p : nd->umap) {
            if(i == inp.size())
                auto_correct_util(p.second, res, inp, str + p.first, i, d+1);
            else if(inp[i] == p.first)
                auto_correct_util(p.second, res, inp, str + p.first, i+1, d);
            else {
                auto_correct_util(p.second, res, inp, str + p.first, i+1, d+1);
                auto_correct_util(p.second, res, inp, str + p.first, i, d+1);
            }
        }
        auto_correct_util(nd, res, inp, str, i+1, d+1);
    }

    vector<string> auto_correct(string inp) {
        unordered_set<string> res;
        auto_correct_util(root, res, inp);
        vector<string> vec = vector<string>(res.begin(), res.end());
        sort(vec.begin(), vec.end());
        return vec;
    }
};

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
    int n;
    cin>>n;
    trie t;
    for(int i=0; i<n; ++i) {
        string str;
        cin>>str;
        t.insert(str);
    }
    
    int c;
    string inp;
    cin>>c>>inp;
    if(c == 1) {
        cout<<t.find(inp);
    } else if(c == 2) {
        vector<string> vec = t.auto_complete(inp);
        cout<<vec.size()<<"\n";
        for(string s : vec)
            cout<<s<<"\n";
    } else if(c == 3) {
        vector<string> vec = t.auto_correct(inp);
        cout<<vec.size()<<"\n";
        for(string s : vec)
            cout<<s<<"\n";
    }
    
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