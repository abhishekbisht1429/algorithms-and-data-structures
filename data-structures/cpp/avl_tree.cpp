#include<iostream>

using namespace std;

template<typename T>
struct node {
    T key;
    int freq;
    node<T> *left;
    node<T> *right;
    int height;
    node(T key): key(key), freq(1), left(nullptr), right(nullptr), height(1) {}
};

template<typename T, typename Compare>
class avl_tree {
    node<T> *root;
    Compare comp;

    int height(node<T> *nd) {
        if(nd == nullptr)
            return 0;
        return nd->height;
    }

    int balance_factor(node<T> *nd) {
        if(nd == nullptr)
            return 0;
        return height(nd->left) - height(nd->right);
    }

    node<T> *rotate_left(node<T> *p) {
        if(p==nullptr || p->right == nullptr)
            return p;
        node<T> *right = p->right;

        p->right = right->left;
        right->left = p;
        p->height = max(height(p->left), height(p->right)) + 1;
        right->height = max(height(right->left), height(right->right)) + 1;

        return right;
    }

    node<T> *rotate_right(node<T> *p) {
        if(p==nullptr || p->left == nullptr)
            return p;
        node<T> *left = p->left;

        p->left = left->right;
        left->right = p;
        p->height = max(height(p->left), height(p->right)) + 1;
        left->height = max(height(left->left), height(left->right)) + 1;

        return left;
    }

    node<T> *insert(node<T> *root, T key) {
        if(root == nullptr)
            return new node<T>(key);
        
        if(!comp(key, root->key) && !comp(root->key, key))
            ++root->freq;
        else if(comp(key, root->key))
            root->left = insert(root->left, key);
        else
            root->right = insert(root->right, key);
        
        root->height = max(height(root->left), height(root->right)) + 1;

        int bf = balance_factor(root);
        node<T> *left = root->left;
        node<T> *right = root->right;
        if(bf > 1 && left != nullptr && comp(key, left->key)) {
            /* 
                node was inserted at left subtree of left child
                fix - right rotate root
            */ 
            root = rotate_right(root);
        } else if(bf > 1 && left != nullptr && comp(left->key, key)) {
            /* 
                node was inserted at right subtree of left child
                fix - left rotate left child
                    - right rotate root
            */
            root->left = rotate_left(root->left);
            root = rotate_right(root);
        } else if(bf < -1 && right != nullptr && comp(right->key, key)) {
            /*
                node was inserted at right subtree of right child
                fix - left rotate root
            */
            root = rotate_left(root);
        } else if(bf < -1 && right != nullptr && comp(key, right->key)) {
            /*
                node was inserted at left subtree of right child
                fix - right rotate right child
                    - left roatate root
            */
            root->right = rotate_right(root->right);
            root = rotate_left(root);
        }

        return root;
    }

    public:void insert(T key) {
        root = insert(root, key);
    }

    node<T> *minVal(node<T> *nd) {
        if(nd == nullptr)
            return nullptr;
        if(nd->left == nullptr)
            return nd;
        else
            return minVal(nd->left);
    }

    node<T> *maxVal(node<T> *nd) {
        if(nd == nullptr)
            return nullptr;
        while(nd->right != nullptr)
            nd = nd->right;
        return nd;
    }

    node<T> *erase(node<T> *root, T key) {
        if(root == nullptr)
            return nullptr;
        
        if(comp(key, root->key)) {
            root->left = erase(root->left, key);
        } else if(comp(root->key, key)) {
            root->right = erase(root->right, key);
        } else if(root->freq > 1) {
            --root->freq;
        } else {
            if(root->right == nullptr) {
                node<T> *temp = root->left;
                delete root;
                root = temp;
            } else if(root->left == nullptr) {
                node<T> *temp = root->right;
                delete root;
                root = temp;
            } else {
                node<T> *succ = minVal(root->right);
                root->key = succ->key;
                root->freq = succ->freq;
                root->right = erase(root->right, succ->key);
            }
        }
            
        if(root != nullptr) {
            root->height = max(height(root->left), height(root->right)) + 1;
            int bf_root = balance_factor(root);
            if(bf_root > 1) {
                /*
                    case R 
                */
                int bf_left = balance_factor(root->left);
                if(bf_left >= 0) {
                    /* 
                        case R0 and R1
                    */
                    root = rotate_right(root);
                } else {
                    /*
                        case R -1
                    */
                    root->left = rotate_left(root->left);
                    root = rotate_right(root);
                }
            } else if(bf_root < -1) {
                /*
                    Case L
                */
                int bf_right = balance_factor(root->right);
                if(bf_right <= 0) {
                    /*
                        case L0 and L -1 
                    */
                    root = rotate_left(root);
                } else {
                    /*
                        case L1
                    */
                    root->right = rotate_right(root->right);
                    root = rotate_left(root);
                }
            }
        }
        return root;
    }

    public:void erase(T key) {
        root = erase(root, key);
    }

    public:bool contains(T key) {
        node<T> *temp = root;
        while(temp != nullptr) {
            if(!comp(key, temp->key) && !comp(temp->key, key))
                return true;
            else if(comp(key, temp->key))
                temp = temp->left;
            else
                temp = temp->right;
        }

        return false;
    }

    public:int frequency(T key) {
        node<T> *temp = root;
        while(temp != nullptr) {
            if(!comp(key, temp->key) && !comp(temp->key, key))
                return temp->freq;
            else if(comp(key, temp->key))
                temp = temp->left;
            else
                temp = temp->right;
        }

        return 0;
    }

    node<T> *lower_bound(node<T> *nd, T key) {
        if(nd == nullptr)
            return nullptr;
        if(!comp(key, nd->key)) {
            if(!comp(nd->key, key))
                return nd;
            return lower_bound(nd->right, key);
        } else {
            node<T> *temp = lower_bound(nd->left, key);
            if(temp!=nullptr && comp(temp->key, nd->key))
                return temp;
            return nd;
        }
    }

    public:T lower_bound(T key, T def_val) {
        node<T> *temp = lower_bound(root, key);
        if(temp != nullptr)
            return temp->key;
        else
            return def_val;
    }

    node<T> *upper_bound(node<T> *nd, T key) {
        if(nd == nullptr)
            return nullptr;
        if(!comp(key, nd->key)) {
            if(!comp(nd->key, key) && nd->right != nullptr)
                return nd->right;
            return upper_bound(nd->right, key);
        } else {
            node<T> *temp = upper_bound(nd->left, key);
            if(temp!=nullptr && comp(temp->key, nd->key))
                return temp;
            return nd;
        }
    }

    public:T upper_bound(T key, T def_val) {
        node<T> *temp = upper_bound(root, key);
        if(temp != nullptr)
            return temp->key;
        else
            return def_val;
    }

    node<T> *kth_largest(node<T> *root, int k) {
        if(root == nullptr)
            return nullptr;
        node<T> *temp = kth_largest(root->right, k);
        cout<<k<<" "<<root->key<<"\n";
        --k;
        if(k<1)
            return temp;
        return kth_largest(root->left, k);
    }

    public:T kth_largest(int k, T def_val) {
        node<T> *temp = kth_largest(root, k);
        if(temp == nullptr)
            return def_val;
        return temp->key;
    }



    void inorder_traversal(node<T> *root) {
        if(root == nullptr)
            return;
        inorder_traversal(root->left);
        cout<<root->key<<" "<<root->freq<<" "<<root->height<<"\n";
        inorder_traversal(root->right);
    }
    
    public:void inorder_traversal() {
        inorder_traversal(this->root);
        cout<<"\n";
    }
};

int main() {
    avl_tree<int, less<int>> tree;
    cout<<"insertion test\n";
    tree.insert(1);
    tree.inorder_traversal();
    tree.insert(2);
    tree.inorder_traversal();
    tree.insert(24);
    tree.inorder_traversal();
    tree.insert(12);
    tree.inorder_traversal();
    tree.insert(12);
    tree.inorder_traversal();
    tree.insert(-1);
    tree.inorder_traversal();
    tree.insert(-3);
    tree.inorder_traversal();
    tree.insert(-3);
    tree.inorder_traversal();
    tree.insert(-3);
    tree.inorder_traversal();

    cout<<"deletion test\n";
    tree.erase(-3);
    tree.inorder_traversal();
    tree.erase(12);
    tree.inorder_traversal();
    tree.erase(12);
    tree.inorder_traversal();
    tree.erase(2);
    tree.inorder_traversal();
    tree.erase(24);
    tree.inorder_traversal();

    tree.insert(-3);
    tree.insert(12);
    tree.insert(12);
    tree.insert(2);
    tree.insert(24);
    cout<<"reinserted deleted values\n";
    tree.inorder_traversal();

    cout<<"contains test\n";
    cout<<tree.contains(3)<<"\n";
    cout<<tree.contains(1)<<"\n";
    cout<<tree.contains(2)<<"\n";
    cout<<tree.contains(-3)<<"\n";
    cout<<tree.contains(12)<<"\n";
    cout<<tree.contains(14)<<"\n";

    cout<<"frequency\n";
    cout<<tree.frequency(3)<<"\n";
    cout<<tree.frequency(1)<<"\n";
    cout<<tree.frequency(2)<<"\n";
    cout<<tree.frequency(-3)<<"\n";
    cout<<tree.frequency(12)<<"\n";
    cout<<tree.frequency(14)<<"\n";

    tree.insert(12);
    tree.insert(-3);
    cout<<tree.frequency(-3)<<"\n";

    tree.inorder_traversal();

    cout<<"lower bound testing\n";
    cout<<tree.lower_bound(-3, -111)<<"\n";
    cout<<tree.lower_bound(-2, -111)<<"\n";
    cout<<tree.lower_bound(5, -111)<<"\n";
    cout<<tree.lower_bound(25, -111)<<"\n";
    cout<<tree.lower_bound(0, -111)<<"\n";
    cout<<tree.lower_bound(-1, -111)<<"\n";
    cout<<tree.lower_bound(1, -111)<<"\n";
    cout<<tree.lower_bound(2, -111)<<"\n";
    cout<<tree.lower_bound(8, -111)<<"\n";
    cout<<tree.lower_bound(12, -111)<<"\n";
    cout<<tree.lower_bound(15, -111)<<"\n";
    cout<<tree.lower_bound(24, -111)<<"\n";

    cout<<"upper bound testing\n";
    cout<<tree.upper_bound(-3, -111)<<"\n";
    cout<<tree.upper_bound(-2, -111)<<"\n";
    cout<<tree.upper_bound(5, -111)<<"\n";
    cout<<tree.upper_bound(12, -111)<<"\n";
    cout<<tree.upper_bound(0, -111)<<"\n";
    cout<<tree.upper_bound(1, -111)<<"\n";
    cout<<tree.upper_bound(25, -111)<<"\n";

    cout<<"kth largest testing\n";
    cout<<tree.kth_largest(1, -111)<<"\n";
}