#include<stdio.h>
#include<stdlib.h>


struct Node{
    int key;
    int h; 
    struct Node *left;
    struct Node *right;
};

//h variable is storing height of individual node, considering that node as root
//so its h = 1 + max(height of lef subtree, height of right subtree)

typedef struct Node NODE;

struct AVLTree{
    NODE* root;
};

typedef struct AVLTree AVL;

AVL* newAVLTree(){
    AVL *t = (AVL*) malloc(sizeof(AVL));
    t->root = NULL;

    return t;
}


//inline this function will sbstitute there where ever we call this 
static inline int max(int x, int y){
    return (x>y)?x:y ;
}

static int height(NODE *t){
    if(t == NULL){
        return 0;
    }
    return t->h;
}

NODE* newAVLNode(int key){
    NODE *t = (NODE*)malloc(sizeof(NODE));
    t->key = key;
    t->h = 1;
    t->left = NULL;
    t->right = NULL;

    return t;
}

static NODE* leftRotation(NODE *t){
    //if t is null
    if(t == NULL){
        return NULL;
    }

    //assuming that right child of "t" in existing 
    NODE *temp = t->right;
    t->right = temp->left;
    temp->left = t;

    //updating heights of t and temp
    t->h = 1 + max(height(t->left), height(t->right));
    temp->h = 1 + max(height(temp->left), height(temp->right));

    return temp;
}

static NODE* rightRotation(NODE *t){
    //if t is null
    if(t == NULL){
        return NULL;
    }

    //assuming that left child of "t" in existing
    NODE *temp = t->left;
    t->left = temp->right;
    temp->right = t; 

    //updating heights of t and temp 
    t->h = 1 + max(height(t->left), height(t->right));
    temp->h = 1 + max(height(temp->left), height(temp->right));    

    return temp;
}

static int balanceFactor(NODE *root){
    if(root == NULL){
        return 0;
    }
    return height(root->left) - height(root->right);
}

static NODE* insert_node(NODE *root, int key){
    //inserting just like in BST 
    if(root == NULL){
        return newAVLNode(key);
    }
    if(key>root->key){
        root->right = insert_node(root->right, key);
    }
    else if(key<root->key){
        root->left = insert_node(root->left, key);
    }
    else{
        return root;
    }

    //updating height 
    root->h = 1+max(height(root->left), height(root->right));

    //now checking balance factor
    int bf = balanceFactor(root);

    if(bf<-1 && key>root->right->key){
        //case: RR
        root = leftRotation(root);
    }
    else if(bf<-1 && key<root->right->key){
        //case: RL
        root->right = rightRotation(root->right);
        root = leftRotation(root);

    }
    else if(bf>1 && key>root->left->key){
        //case: LR
        root->left = leftRotation(root->left);
        root = rightRotation(root);
    }
    else if(bf>1 && key<root->left->key){
        //case: LL
        root = rightRotation(root);
    }
    

    return root; 
}

void avlInsertion(AVL *t, int key){
    t->root = insert_node(t->root, key);
}

static void inorderTraversal(NODE *root){
    if(root == NULL){
        return;
    }
    inorderTraversal(root->left);
    printf("%d ", root->key);
    inorderTraversal(root->right);
}

void inorderAVLTraversal(AVL* t){
    inorderTraversal(t->root);
}

static NODE* minval_node(NODE *root, int key){
    if(root == NULL){
        return NULL;
    }
    else if(root->left == NULL){
        return root;
    }
    return minval_node(root->left, key);
}

static NODE* delete(NODE *root ,int key){
    //BST deletion 
    if(root == NULL){
        return root;
    }

    if(key>root->key){
        root->right = delete(root->right, key);
    }
    else if(key<root->key){
        root->left = delete(root->left, key);
    }
    else{
        //if single child or no child 
        if(root->right == NULL){
            NODE *temp = root->left;
            free(root);
            root = temp;
        }
        else if(root->left == NULL){
            NODE *temp = root->right;
            free(root);
            root = temp;
        }
        else{
            //if two child
            NODE *suc = minval_node(root->right, key);
            root->key = suc->key;
            root->right = delete(root->right, suc->key);
        }
        
    }

    //AVL procedure
   
    if(root!=NULL){
        root->h = 1+max(height(root->left), height(root->right));

        int bf = balanceFactor(root);

        if(bf>1){
            //deletion from right subtree
            int bfl = balanceFactor(root->left);
            if(bfl>=0){
                root = rightRotation(root);
            }
            else{
                root->left = leftRotation(root->left);
                root = rightRotation(root);
            }
        }
        else if(bf<-1){
            //deletion from left subtree
            int bfr = balanceFactor(root->right);
            if(bfr<=0){
                root = leftRotation(root);
            }
            else{
                root->right = rightRotation(root->right);
                root = leftRotation(root);
            }
        }
    }

    return root;
}

void avlDeletion(AVL* t, int key){
    t->root = delete(t->root, key);
}

// int main(){

//     AVL *t = newAVLTree();
    
//     avlInsertion(t, 12);
//     avlInsertion(t, 90);
//     avlInsertion(t, 8);
//     avlInsertion(t, 20);
//     avlInsertion(t, 13);
//     avlInsertion(t, 15);
//     avlInsertion(t, 19);
//     avlInsertion(t, 62);
//     avlInsertion(t, 70);
//     avlInsertion(t, 45);

//     inorderAVLTraversal(t);
//     printf("\n");

//     avlDeletion(t, 90);
//     avlDeletion(t, 20);
//     avlDeletion(t, 8);

//     inorderAVLTraversal(t);
//     printf("\n");

//     return 0;
// }