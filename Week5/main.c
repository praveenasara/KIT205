#include <stdio.h>
#include "bst.h"
#include "bst_print.h" // Add this to use the new visualization tools

int main() {
    BST tree = new_bst();
    
    // Insert some sample data
    insert_bst(&tree, 50);
    insert_bst(&tree, 30);
    insert_bst(&tree, 70);
    insert_bst(&tree, 20);
    insert_bst(&tree, 40);

    // Use the NEW provided visualization functions
    printf("Visual Hierarchy:");
    print_bst_visual(&tree); // From bst_print.c

    printf("\nSpatial 2D Tree Layout:");
    print_bst_2d(&tree);     // From bst_print.c

    // Run your existing tests
    bst_test();

    destroy_bst(&tree);
    return 0;
}