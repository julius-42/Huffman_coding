#include "structs.h"


#define MAX_HEIGHT 10
#define MAX_WIDTH 128


// Huffman Tree functions

Node* create_node(int val){
    Node* node = malloc(sizeof(Node));
    if(!node) return NULL;
    node->frq = val;
    node->c = '\0';
    node->left = NULL;
    node->right = NULL;
    return node;
}

void free_tree(Node* node){
    if(!node) return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}


// Priority queue functions

ListNode* create_list_node(Node* data){
    ListNode* listnode = malloc(sizeof(ListNode));
    if(!listnode) return NULL;
    listnode->data = data;
    listnode->next = NULL;
    return listnode;
}

void free_list(ListNode* list){
    if(!list) return;
    free_list(list->next);
    free(list);
}

ListNode* insert_list_node(ListNode* list, ListNode* node){
    int val = node->data->frq;
    
    // empty list
    if(!list) return node;

    // insert at head
    if(val <= list->data->frq){
        node->next = list;
        return node;
    }

    // find insertion point
    ListNode* curr = list;
    while(curr->next != NULL && val > curr->next->data->frq){
        curr = curr->next;
    }

    // insert between nodes
    node->next = curr->next;
    curr->next = node;

    return list;
}



// Tree printing functions

void print_node_value(Node* node) {
    if (node == NULL) {
        printf("NULL");
        return;
    }
    // If it's a printable character, show both freq and char, otherwise just freq
    if (node->c >= 32 && node->c <= 126) {
        printf("(%d:'%c')", node->frq, node->c);
    } else {
        printf("(%d)", node->frq);
    }
}

// Breadth-First Search printing function
void print_bfs_triplets(Node* root) {
    if (root == NULL) {
        printf("Tree is empty.\n");
        return;
    }

    // Create a simple queue for level-order traversal
    // Increase 1024 if you are dealing with massive trees
    Node* queue[1024]; 
    int head = 0;
    int tail = 0;

    // Enqueue the root
    queue[tail++] = root;

    printf("--- BFS Level-Order (Left - Parent - Right) ---\n");

    while (head < tail) {
        // Dequeue the current parent node
        Node* parent = queue[head++];

        // Print in the requested format: "left - parent - right"
        print_node_value(parent->left);
        printf(" - ");
        print_node_value(parent);
        printf(" - ");
        print_node_value(parent->right);
        printf("\n");

        // Enqueue children for the next levels
        if (parent->left != NULL) {
            queue[tail++] = parent->left;
        }
        if (parent->right != NULL) {
            queue[tail++] = parent->right;
        }
    }
    printf("-----------------------------------------------\n");
}