#include <stdlib.h>
#include <stdio.h>
#include "bst.h"
#include "bst_print.h" 

// create an empty bst
BST new_bst() {
	BST temp;
	temp.root = NULL;
	return temp;
}

// recursive function to find a value and return the containing node
BSTNodePtr find_bst_node(BSTNodePtr self, int n) {
	if (self == NULL || (n == self->data_item)) { /* not present, or here */
		return self;
	} else if (n < self->data_item) {
		return find_bst_node(self->left, n);
	} else {
		return find_bst_node(self->right, n);
	}
}

// find a value in the tree and return the node
BSTNodePtr find_bst(BST* self, int n) {
	return find_bst_node(self->root, n);
}

// recursive function to insert a value
BSTNodePtr insert_bst_node(BSTNodePtr self, int n) {
	if (self == NULL) { /* found where to put it*/
		self = malloc(sizeof *self);
		self->data_item = n;
		self->left = self->right = NULL;
	} else if (n < self->data_item) {
		self->left = insert_bst_node(self->left, n);
	} else if (n >self->data_item) {
		self->right = insert_bst_node(self->right, n);
	}
	return self;
}

// insert a value into the tree
void insert_bst(BST* self, int n) {
	self->root = insert_bst_node(self->root, n);
}

// helper function for delete
BSTNodePtr min_node(BSTNodePtr self) {
	BSTNodePtr current = self;
	while (current->left != NULL) {
		current = current->left;
	}
	return current;
}

// recursive function to delete a value
BSTNodePtr delete_bst_node(BSTNodePtr self, int n) {
	if (self != NULL) {
		if (n == self->data_item) { // found item 
			if (self->left != NULL && self->right != NULL) {
				// two child case 
				BSTNodePtr successor = min_node(self->right);
				self->data_item = successor->data_item;
				self->right = delete_bst_node(self->right, self->data_item);
			} else { // one or zero child case 
				BSTNodePtr to_free = self;
				if (self->left) {
					self = self->left;
				} else {
					self = self->right;
				}
				free(to_free);
			}
		} else if (n < self->data_item) {
			self->left = delete_bst_node(self->left, n);
		} else {
			self->right = delete_bst_node(self->right, n);
		}
	}
	return self;
}

// delete a value from the tree
void delete_bst(BST* self, int n) {
	self->root = delete_bst_node(self->root, n);
}

// recursive function to print in order
void print_in_order_bst_node(BSTNodePtr self){
	if (self == NULL) {
		printf("_");
	} else {
		printf("(");
		print_in_order_bst_node(self->left);
		printf(" %d ", self->data_item);
		print_in_order_bst_node(self->right);
		printf(")");
	}
}

// print the tree in order
void print_in_order_bst(BST *self) {
	print_in_order_bst_node(self->root);
}

// 1. Recursive helper (Hidden from bst.h)
void print_pre_order_bst_node(BSTNodePtr self) {
    if (self == NULL) {
        printf("_");
    } else {
        printf("(");
        printf(" %d ", self->data_item); // Print Root first
        print_pre_order_bst_node(self->left);
        print_pre_order_bst_node(self->right);
        printf(")");
    }
}

// 2. Public wrapper
void print_pre_order_bst(BST *self) {
    print_pre_order_bst_node(self->root);
}

// 3. Recursive helper (Hidden from bst.h)
void print_post_order_bst_node(BSTNodePtr self) {
    if (self == NULL) {
        printf("_");
    } else {
        printf("(");
        print_post_order_bst_node(self->left);
        print_post_order_bst_node(self->right);
        printf(" %d ", self->data_item); // Print Root last
        printf(")");
    }
}

// 4. Public wrapper
void print_post_order_bst(BST *self) {
    print_post_order_bst_node(self->root);
}

// recursive function to detroy all node
void destroy_bst_node(BSTNodePtr self) {
	if (self != NULL) {
		destroy_bst_node(self->left);
		self->left = NULL;
		destroy_bst_node(self->right);
		self->right = NULL;
		free(self);
	}
}

// destroy the tree
void destroy_bst(BST *self) {
	destroy_bst_node(self->root);
	self->root = NULL;
}

// modified bst_adhoc_test
void bst_adhoc_test() {	
    BST tree = new_bst();	
    int data;	
    
    printf("Enter data (0 to stop):\n");
    while (scanf("%d", &data) && data != 0) {		
        insert_bst(&tree, data);		
    }	

    printf("\nIn-order:   "); 
    print_in_order_bst(&tree);
    
    printf("\nPre-order:  "); 
    print_pre_order_bst(&tree);
    
    printf("\nPost-order: "); 
    print_post_order_bst(&tree);
    printf("\n");

    destroy_bst(&tree);
}

// this function will systematically check insertion

void bst_test() {
    printf("\n--- Advanced BST Unit Tests ---\n");
    BST tree = new_bst();

    // 1. Building the tree
    insert_bst(&tree, 50); insert_bst(&tree, 30); insert_bst(&tree, 70);
    insert_bst(&tree, 20); insert_bst(&tree, 40);

    // 2. Using the provided 2D print instead of the messy one
    printf("Spatial Tree Structure:\n");
    print_bst_2d(&tree); // This is the provided function from bst_print.c

    // 3. Testing your Height function
    printf("Calculated Height: %d (Expected: 2)\n", height_bst(&tree));

    // 4. Testing Deletion
    printf("\nDeleting node 30 (Node with children)...\n");
    delete_bst(&tree, 30);
    print_bst_2d(&tree); // Visualize the change

    destroy_bst(&tree);
    printf("\n--- Full Test Complete ---\n");
}
// Helper function to find the maximum of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Recursive function to calculate height
int height_bst_node(BSTNodePtr self) {
    if (self == NULL) {
        return -1; // Base case: empty tree has height -1
    } else {
        // Height is 1 + the height of the taller subtree
        return 1 + max(height_bst_node(self->left), 
                       height_bst_node(self->right));
    }
}

// Public wrapper function
int height_bst(BST *self) {
    return height_bst_node(self->root);
}