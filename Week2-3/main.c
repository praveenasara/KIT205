#include <stdio.h>
#include "list.h"

// Comment out or keep your original test function
void list_test() {
    // ... your original test code here (commented out for now)
    printf("Original list_test is commented out.\n");
}

int main() {
    // 1. Keep your interactive menu so you can still test manually
    // After you type '0' to exit the menu, the code below will run.
    list_adhoc_test();
    
    printf("\n--- Starting Final Task Tests ---\n");

    // 2. Test the Reverse function
    List list1 = new_list();
    insert_at_front(&list1, 30);
    insert_at_front(&list1, 20);
    insert_at_front(&list1, 10);
    
    printf("Original List 1: "); 
    print_list(&list1);

    List rev = reverse(&list1); 
    printf("Reversed List:   "); 
    print_list(&rev);

    // 3. Test the Merge function
    List list2 = new_list();
    insert_in_order(&list2, 25);
    insert_in_order(&list2, 15);
    
    printf("Original List 2: "); 
    print_list(&list2);

    List merged = merge(&list1, &list2); 
    printf("Merged List:     "); 
    print_list(&merged);

    // 4. Cleanup memory (Crucial!)
    destroy_list(&list1);
    destroy_list(&list2);
    destroy_list(&rev);  
    destroy_list(&merged);
    
    return 0;
}