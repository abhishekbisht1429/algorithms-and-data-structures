#include<stdio.h>
#include<stdlib.h>

struct Tnode{
    int data;
    struct Tnode *left;
    struct Tnode *right;
};

typedef struct Tnode TNODE;

struct bst{
    TNODE *root;
    int height;
};

typedef struct bst BST; 

BST* new_bst() {
    BST *tree = (BST*)malloc(sizeof(BST));
    tree->root = NULL;
    tree->height = 0;

    return tree;
}

void insert_recur(TNODE **parent, int data) {
    if((*parent) == NULL) {
        TNODE *r = (TNODE*)malloc(sizeof(TNODE));
        r->data = data;
        r->left = NULL;
        r->right = NULL;
        *parent = r;
    } else if(data <= (*parent)->data) {
        insert_recur(&((*parent)->left), data);
    } else {
        insert_recur(&((*parent)->right), data);
    }
}

void traversal_inorder(TNODE *parent){

    if(parent == NULL){
        return;
    }

    traversal_inorder(parent->left);
    printf("%d ", parent->data);
    traversal_inorder(parent->right);
}

TNODE *minVal(TNODE *node) {
    TNODE *current = node; 
  
    while (current && current->left != NULL) 
        current = current->left; 
  
    return current; 
}

/*algorithm deletion*/
/*
    This function will return the root element after deletion
    1. search for the location of key in the tree
    2. three cases 
        a.) key is greater than current node - recursively call deletion for right subtree
        b.) key is less than current node - recursively call deletion for left subtree
        c.) key is equal to current node - 
            i) no children and one child 
            ii) find inorder successor, and 
                replace value of current node by it, 
                recursively call deletion on inorder successor


    3
     \
      6

    delete(root(3), 6)
    delete(root(6), 6) - temp = root->right = NULL

*/

TNODE *delete_recur(TNODE *root, int data) {
    if(root == NULL) {
        // printf("null\n");
        return NULL;
    } else if(data < root->data) {
        // printf("else if left %d\n", root->data);
        root->left = delete_recur(root->left, data);
    } else if(data > root->data) {
        // printf("else if right %d\n", root->data);
        root->right = delete_recur(root->right, data);
    } else {
        // printf("else\n");
        if(root->left == NULL) {
            TNODE *temp = root->right;
            free(root);
            return temp;
        } else if(root->right == NULL) {
            TNODE *temp = root->left;
            free(root);
            return temp;
        } else {
            TNODE *successor = minVal(root->right);
            root->data = successor->data;

            root->right = delete_recur(root->right, successor->data);
        }
    }
    return root;
}

void delete(BST *tree, int data) {
    tree->root = delete_recur(tree->root, data);
}

int main(){
    BST *tree = new_bst();
    

    insert_recur(&(tree->root), 10);
    insert_recur(&(tree->root), 2);
    insert_recur(&(tree->root), 3);
    insert_recur(&(tree->root), 1);
    insert_recur(&(tree->root), 5);

    traversal_inorder(tree->root);
    printf("\n");

    delete(tree, 3);

    traversal_inorder(tree->root);
    printf("\n");

    return 0;
}

