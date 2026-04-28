#include <stdio.h>
#include <stdlib.h>

// Data Structure Definition
enum Color {RED , BLACK};
struct node {
    int data;
    enum Color color;
    struct node *left, *right, *parent;
};
// Global root for the whole tree
struct node* root = NULL;

// Function Prototypes
struct node* search(struct node* root, int data);
struct node* BSTInsert(struct node* trav, struct node* temp);
struct node* minNode(struct node* node);
void replaceNode(struct node* u, struct node* v);
void leftRotate(struct node* temp);
void rightRotate(struct node* temp);
void fixInsert(struct node* pt);
void fixDelete(struct node* x);
void freeTree(struct node* node);

// Rotation Methods
void rightRotate (struct node* temp){
    struct node* left = temp->left;
    temp->left = left->right;

    if (temp->left) 
        temp->left->parent = temp;
    left->parent = temp->parent;
    if (!temp->parent) 
        root = left;
    else if (temp == temp->parent->left) 
        temp->parent->left = left;
    else 
        temp->parent->right = left;
    left->right = temp;
    temp->parent = left;
}

void leftRotate (struct node* temp){
    struct node* right = temp->right;
    temp->right = right->left;

    if (temp->right) 
        temp->right->parent = temp;
    right->parent = temp->parent;
    if (!temp->parent) 
        root = right;
    else if (temp == temp->parent->left) 
        temp->parent->left = right;
    else 
        temp->parent->right = right;
    right->left = temp;
    temp->parent = right;
}

// Insertion ----------------------------------------------------
// BST Logic for insertion
struct node* BSTInsert(struct node* trav, struct node* temp) {
    if (trav == NULL) return temp;
    if (temp->data < trav->data) {
        trav->left = BSTInsert(trav->left, temp);
        trav->left->parent = trav;
    } else if (temp->data > trav->data) {
        trav->right = BSTInsert(trav->right, temp);
        trav->right->parent = trav;
    }
    return trav;
}
// Fix the violations caused by BST logic after inserting
void fixInsert(struct node* pt){
    struct node* parent_pt = NULL;
    struct node* grandparent_pt = NULL;

    while (pt != root && pt->parent->color == RED){
        parent_pt = pt->parent;

        if (parent_pt->parent == NULL) break;
        grandparent_pt = parent_pt->parent;

        // Case A:
        // Parent of pt = left child of pt's grandparent
        if (parent_pt == grandparent_pt->left){
            struct node* uncle_pt = grandparent_pt->right;
            // Scenario 1:
            // The uncle of pt is red, recoloring needed
            if (uncle_pt != NULL && uncle_pt->color == RED){
                grandparent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grandparent_pt;
            }
            else {
                // Scenario 2:
                // pt is right child of its parent, need left rotation
                if (pt == parent_pt->right){
                    leftRotate(parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
                // Scenario 3:
                // pt is left child of its parent, need right rotation
                rightRotate(grandparent_pt);
                enum Color c = parent_pt->color;
                parent_pt->color = grandparent_pt->color;
                grandparent_pt->color = c;

                pt = parent_pt;
            }
        }
        // Case B:
        // Parent of pt = right child of pt's grandparent
        else {
            struct node* uncle_pt = grandparent_pt->left;
            // Scenario 1:
            // The uncle of pt is red, recoloring needed
            if (uncle_pt != NULL && uncle_pt->color == RED){
                grandparent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grandparent_pt;
            }
            else {
                // Scenario 2:
            // pt is left child of its parent, need right rotation
                if (pt == parent_pt->left){
                    rightRotate(parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
                // Scenario 3:
                // pt is right child of its parent, need left rotation
                leftRotate(grandparent_pt);
                enum Color c = parent_pt->color;
                parent_pt->color = grandparent_pt->color;
                grandparent_pt->color = c;
                pt = parent_pt;
            }
        }
    }
    root->color = BLACK;
}

// Wrapper function for entire insertion process
void insert(int data) {
    if (root) root->color = BLACK;
    struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->data = data;
    temp->left = temp->right = temp->parent = NULL;
    temp->color = RED;
    root = BSTInsert(root, temp);
    fixInsert(temp);
}
// ---------------------------------------------------------------

// Deletion ----------------------------------------------------
void delete(int data){
    struct node* z = search(root, data);
    if (z == NULL) return;

    struct node *x, *y;
    y = z;
    enum Color y_original_color = y->color;

    if (z->left == NULL) {
        x = z->right;
        replaceNode(z, z->right);
    } else if (z->right == NULL) {
        x = z->left;
        replaceNode(z, z->left);
    } else {
        y = minNode(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            if (x) x->parent = y;
        } else {
            replaceNode(y, y->right);
            y->right = z->right;
            if (y->right) y->right->parent = y;
        }
        replaceNode(z, y);
        y->left = z->left;
        if (y->left) y->left->parent = y;
        y->color = z->color;
    }
    if (y_original_color == BLACK) fixDelete(x);
    free(z);
}
// Fix the violations caused by BST logic after deleting
void fixDelete(struct node* x) {
    while (x != root && (x == NULL || x->color == BLACK)) {

        struct node* parent = (x != NULL) ? x->parent : NULL;
        if (parent == NULL) break;

        if (x == parent->left) {
            struct node* s = parent->right;

            // Case 1: sibling red
            if (s && s->color == RED) {
                s->color = BLACK;
                parent->color = RED;
                leftRotate(parent);
                s = parent->right;
            }

            // Case 2: sibling black with black children
            if ((s == NULL) ||
                ((s->left == NULL || s->left->color == BLACK) &&
                 (s->right == NULL || s->right->color == BLACK))) {

                if (s) s->color = RED;
                x = parent;
            } else {
                // Case 3: Sibling black, left child red, right child black
                if (s->right == NULL || s->right->color == BLACK) {
                    if (s->left) s->left->color = BLACK;
                    if (s) s->color = RED;
                    rightRotate(s);
                    s = parent->right;
                }

                // Case 4: Sibling black, right child red
                if (s) s->color = parent->color;
                parent->color = BLACK;
                if (s && s->right) s->right->color = BLACK;
                leftRotate(parent);
                x = root;
            }
        } else {
            // Symmetric/mirror case
            struct node* s = parent->left;

            if (s && s->color == RED) {
                s->color = BLACK;
                parent->color = RED;
                rightRotate(parent);
                s = parent->left;
            }

            if ((s == NULL) ||
                ((s->left == NULL || s->left->color == BLACK) &&
                 (s->right == NULL || s->right->color == BLACK))) {

                if (s) s->color = RED;
                x = parent;
            } else {
                if (s->left == NULL || s->left->color == BLACK) {
                    if (s->right) s->right->color = BLACK;
                    if (s) s->color = RED;
                    leftRotate(s);
                    s = parent->left;
                }

                if (s) s->color = parent->color;
                parent->color = BLACK;
                if (s && s->left) s->left->color = BLACK;
                rightRotate(parent);
                x = root;
            }
        }
    }

    if (x) x->color = BLACK;
}
// ----------------------------------------------------------

// Searching Method
struct node* search(struct node* root, int data) {
    if (root == NULL || root->data == data) return root;
    if (data < root->data) return search(root->left, data);
    return search(root->right, data);
}

// Traversal Method (Left, Root, Right)
void inorder(struct node* root){
    if (root == NULL) return;
    inorder(root->left);
    printf("%d (%s) ", root->data, root->color == RED ? "R" : "B");
    inorder(root->right);
}

// Other Helper Methods ---------------------------------------
struct node* minNode(struct node* node){
    while (node->left != NULL) node = node->left;
    return node;
}

// Swap nodes
void replaceNode(struct node* u, struct node* v) {
    if (u->parent == NULL) root = v;
    else if (u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;
    if (v != NULL) v->parent = u->parent;
}

// Frees memory, leaves to root
void freeTree(struct node* node) {
    if (node == NULL) return;
    // Handle subtrees
    freeTree(node->left);
    freeTree(node->right);
    // Free current node
    free(node);
}

int getBlackHeight(struct node* n) {
    if (n == NULL) return 1;
    int leftHeight = getBlackHeight(n->left);
    int rightHeight = getBlackHeight(n->right);
    if (leftHeight == -1 || rightHeight == -1 || leftHeight != rightHeight) return -1;
    return leftHeight + (n->color == BLACK ? 1 : 0);
}

// Verifies validity of red-black tree
int verifyRB(struct node* n) {
    if (n == NULL) return 1;
    if (n->color == RED) {
        if ((n->left && n->left->color == RED) || (n->right && n->right->color == RED)) return 0;
    }
    return verifyRB(n->left) && verifyRB(n->right);
}

//-----------------------------------------------------------
// TEST SUITE------------------------------------------------
void runTestSuite() {
    
    printf("Red-Black Tree Test Suite:\n\n");

    // Test 1: Basic Insertion & Balancing
    printf("Test 1: Sequential Insertion (1 to 10)... ");
    for(int i = 1; i <= 10; i++) insert(i);
    if (root->color == BLACK && getBlackHeight(root) != -1 && verifyRB(root)) 
        printf("PASSED\n");
    else printf("FAILED\n");

    // Test 2: Search Functionality
    printf("Test 2: Searching for existing and non-existing nodes... ");
    struct node* found = search(root, 5);
    struct node* notFound = search(root, 99);
    if (found != NULL && found->data == 5 && notFound == NULL)
        printf("PASSED\n");
    else printf("FAILED\n");

    // Test 3: Deletion - Leaf Node
    printf("Test 3: Deleting a leaf node (1)... ");
    delete(1);
    if (search(root, 1) == NULL && verifyRB(root))
        printf("PASSED\n");
    else printf("FAILED\n");

    // Test 4: Deletion - Root Node
    printf("Test 4: Deleting the root... ");
    int oldRootData = root->data;
    delete(oldRootData);
    if (search(root, oldRootData) == NULL && root->color == BLACK && verifyRB(root))
        printf("PASSED\n");
    else printf("FAILED\n");

    // Test 5: Random RB-Tree Build
    printf("Test 5: Random RB-Tree... ");
    // Clear tree first
    freeTree(root);
    root = NULL;
    for(int i = 0; i < 10; i++) {
        insert(rand() % 1000);
    }
    if (verifyRB(root) && getBlackHeight(root) != -1)
        printf("PASSED\n");
    else printf("FAILED\n");

    freeTree(root);
    root = NULL;
}
//-----------------------------------------------------------
//-----------------------------------------------------------

// Main method
int main(){

    // Run Test Suite
    runTestSuite();
    return 0;
}