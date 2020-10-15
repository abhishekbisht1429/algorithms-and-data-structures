#include<stdio.h>
#include<stdlib.h>

struct Node {
    int key;
    struct Node *left;
    struct Node *right;
    int height;
};
typedef struct Node NODE;

struct Avl {
    NODE *root;
};

typedef struct Avl AVL;

NODE *newAVLNode(int key) {
    NODE *node = (NODE*)malloc(sizeof(NODE));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;

    return node;
}

AVL* newAVLTree(){
    AVL *t = (AVL*) malloc(sizeof(AVL));
    t->root = NULL;

    return t;
}

static int height(NODE *node) {
    if(node == NULL) {
        return 0;
    } else {
        return node->height;
    }
}

static int balance_factor(NODE *node) {
    if(node == NULL) {
        return 0;
    } else {
        return height(node->left) - height(node->right);
    }
}

static inline int max(int x, int y) {
    return x>y?x:y;
}

//NOTE: the caller to this function will ensure that node->right is not null
static NODE *rotateLeft(NODE *node) {
    // printf("rotating left %d\n", node->key);
    if(node == NULL){
        return NULL;
    }
    NODE *root = node->right;
    node->right = root->left;
    root->left = node;

    node->height = max(height(node->left), height(node->right)) + 1;
    root->height = max(height(root->left), height(root->right)) + 1;

    return root;
}

//NOTE: the caller to this function will ensure that node->left is not null
static NODE *rotateRight(NODE *node) {
    // printf("rotating right %d\n", node->key);
    if(node == NULL) {
        return NULL;
    }

    NODE *root = node->left;
    node->left = root->right;
    root->right = node;

    node->height = max(height(node->left), height(node->right)) + 1;
    root->height = max(height(root->left), height(root->right)) + 1;

    return root;
}

static NODE *insert(NODE *root, int key) {
    if(root == NULL) {
        return newAVLNode(key);
    }
    if(key < root->key) {
        // printf("left\n");
        root->left = insert(root->left, key);
    } else {
        // printf("right\n");
        root->right = insert(root->right, key);
    }

    root->height = max(height(root->left), height(root->right)) + 1;

    int balance = balance_factor(root);

    //left subtree of left child
    //rotate right
    if(balance > 1 && key < root->left->key) {
        root = rotateRight(root);
    } else if(balance > 1 && key > root->left->key) { 
        //right subtree of left child --> rotate left child towards left
        // and then right rotate the root
        root->left = rotateLeft(root->left);
        root = rotateRight(root);
    } else if(balance < -1 && key > root->right->key) {
        //right subree of right child --> rotate left
        root = rotateLeft(root);
    } else if(balance < -1 && key < root->right->key) {
        //left subtree of right child --> rotate right child towards right
        //and then rotate the root towards left
        root->right = rotateRight(root->right);
        root = rotateLeft(root);
    }
    return root;
}

void avlInsert(AVL *tree, int key) {
    tree->root = insert(tree->root, key);
}

static NODE *minVal(NODE *node) {
    if(node == NULL) {
        return NULL;
    }
    if(node->left == NULL) {
        return node;
    } else {
        return minVal(node->left);
    }
}

static NODE *delete(NODE *root, int key) {
    if(root == NULL) {
        return NULL;
    }
    if(key < root->key) {
        root->left = delete(root->left, key);
    } else if(key > root->key) {
        root->right = delete(root->right, key);
    } else {
        if(root->right == NULL) {
            NODE *temp = root->left;
            free(root);
            root = temp;
        } else if(root->left == NULL) {
            NODE *temp = root->right;
            free(root);
            root = temp;
        } else {
            NODE *succ = minVal(root->right); // a -> b == value of b at address stored in a // *a == value at address stored by a
            root->key = succ->key;
            root->right = delete(root->right, succ->key);
        }

        if(root!=NULL) {
            root->height = 1 + max(height(root->left), height(root->right));
            int bf_root = balance_factor(root);
            if(bf_root > 1) { //case R
                int bf_left = balance_factor(root->left);
                if(bf_left >= 0) { //R0 and R1 case
                    root = rotateRight(root);
                } else { //R-1 case
                    root->left = rotateLeft(root->left);
                    root = rotateRight(root);
                }
            } else if(bf_root < -1) {
                int bf_right = balance_factor(root->right);
                if(bf_right <= 0) { //L0 and L-1 case
                    root = rotateLeft(root);
                } else {
                    root->right = rotateRight(root->right);
                    root = rotateLeft(root);
                }
            }
        }
    }
    return root;
}

void avlDelete(AVL *tree, int key) {
    tree->root = delete(tree->root, key);
}

void inorder(NODE *root) {
    if(root == NULL) {
        return;
    }
    inorder(root->left);
    printf("%d ", root->key);
    inorder(root->right);
}

void avlInorder(AVL *tree) {
    inorder(tree->root);
}

int main() {
    AVL *tree = newAVLTree();
    printf("avlInsert %d\n",12);
    avlInsert(tree, 12);
    printf("height : %d\nkey : %d\n\n", tree->root->height, tree->root->key);

    printf("avlInsert %d\n",90);
    avlInsert(tree, 90);
    printf("height : %d\nkey : %d\n\n", tree->root->height, tree->root->key);

    printf("avlInsert %d\n",8);
    avlInsert(tree, 8);
    printf("height : %d\nkey : %d\n\n", tree->root->height, tree->root->key);

    printf("avlInsert %d\n",20);
    avlInsert(tree, 20);
    printf("height : %d\nkey : %d\n\n", tree->root->height, tree->root->key);

    printf("avlInsert %d\n",13);
    avlInsert(tree, 13);
    printf("height : %d\nkey : %d\n\n", tree->root->height, tree->root->key);

    printf("avlInsert %d\n",15);
    avlInsert(tree, 15);
    printf("height : %d\nkey : %d\n\n", tree->root->height, tree->root->key);

    printf("avlInsert %d\n",19);
    avlInsert(tree, 19);
    printf("height : %d\nkey : %d\n\n", tree->root->height, tree->root->key);

    printf("avlInsert %d\n",62);
    avlInsert(tree, 62);
    printf("height : %d\nkey : %d\n\n", tree->root->height, tree->root->key);
    
    printf("avlInsert %d\n",70);
    avlInsert(tree, 70);
    printf("height : %d\nkey : %d\n\n", tree->root->height, tree->root->key);

    printf("avlInsert %d\n",45);
    avlInsert(tree, 45);
    printf("height : %d\nkey : %d\n\n", tree->root->height, tree->root->key);

    avlInsert(tree, 43);
    avlInsert(tree, 30);
    avlInsert(tree, 34);
    avlInsert(tree, 67);
    avlInsert(tree, 100);

    avlInorder(tree);
    printf("\n");

    avlDelete(tree, 43);
    avlDelete(tree, 19);
    avlDelete(tree, 8);
    avlDelete(tree, 100);

    avlInorder(tree);
    printf("\n");
    return 0;
}