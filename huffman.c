#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

// Count each char frequency in str
// Put frequencies at frq_arr[char]
void count_frqs(char* str, int frq_arr[]){
    for(int i = 0; str[i] != '\0'; i++){
        frq_arr[(unsigned char)str[i]]++;
    }
}

// Put every char with its frequency into Prior. Q 
// and merge them into new nodes with sum of frequencies and char '\0'
// returns the root of the Huffman tree
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

// Track all chars in the tree and insert their encoded version into coded_arr[char]
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


int main(int argc, char* argv[]){
    char* to_encode;

    if(argc == 2){
        to_encode = argv[1];
    }
    else{
        to_encode = "AAAAABBBCCD";
    }

    Node* root = create_huffman_tree(to_encode);
    print_bfs_triplets(root);
    print_coded(root);

    return 1;
}