#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <stdlib.h>

// Huffman tree - non-char nodes use '\0'
typedef struct Node {
    int frq;
    char c;
    struct Node* left;
    struct Node* right;
} Node;

Node* create_node(int val);

void free_tree(Node* node);

void print_node_value(Node* node);
void print_bfs_triplets(Node* root);


// Priority queue - lowest freq first
typedef struct ListNode {
    struct ListNode* next;
    struct Node* data;
} ListNode;

ListNode* create_list_node(Node* data);

void free_list(ListNode* list);

ListNode* insert_list_node(ListNode* list, ListNode* node);

#endif