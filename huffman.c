#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HEIGHT 10
#define MAX_WIDTH 128

// Huffman tree
typedef struct Node {
    int frq;
    char c;
    struct Node* left;
    struct Node* right;
} Node;

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

// Linked List
typedef struct ListNode {
    struct ListNode* next;
    struct Node* data;
} ListNode;

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


void count_frqs(char* str, int frq_arr[]){
    for(int i = 0; str[i] != '\0'; i++){
        frq_arr[(unsigned char)str[i]]++;
    }
}

Node* create_huffman_tree(char str[]){
    int frq_arr[256] = {0};
    count_frqs(str, frq_arr);
    ListNode* list_head = NULL;

    // Create Priority Queue from lowest freq to highest
    for(int i = 0; i < 256; i++){
        int cur_frq = frq_arr[i];
        if(cur_frq > 0){

            Node* node = create_node(cur_frq);
            node->c = (char)i;
            ListNode* list_node = create_list_node(node);
            list_head = insert_list_node(list_head, list_node);
        }
    }


    // Form Huffman Tree
    ListNode* curr = list_head;
    while(curr->next != NULL){
        Node* parent = create_node(curr->data->frq + curr->next->data->frq);
        ListNode* l_node = create_list_node(parent);
        
        parent->left = curr->data;
        parent->right = curr->next->data;

        list_head = insert_list_node(list_head, l_node);
        
        curr = curr->next->next;
    }

    Node* root = curr->data;
    free_list(list_head);

    return root;

}

void encode_chars(Node* node, char coded_arr[256][128], char* code) {
    
    if(!node->left && !node->right){
        strcpy(coded_arr[(unsigned char)node->c], code);
        return;
    }

    char left_code[128];
    char right_code[128];

    strcpy(left_code, code);
    strcat(left_code, "0");
    encode_chars(node->left, coded_arr, left_code);

    strcpy(right_code, code);
    strcat(right_code, "1");
    encode_chars(node->right, coded_arr, right_code);
}

void print_coded(Node* root){

    char coded_arr[256][128] = {0};

    encode_chars(root, coded_arr, "");

    for(int i = 0; i < 256; i++){
        if(coded_arr[i][0] != '\0'){
            printf("%c : %s\n", (char)i, coded_arr[i]);
        }
    }
}




// PRINT STUFF

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

int main(int argc, char* argv[]){
    Node* root = create_huffman_tree("AAAAAAAAAABBBBBCCCDDEEF");
    //print_bfs_triplets(root);
    print_coded(root);

    return 1;
}