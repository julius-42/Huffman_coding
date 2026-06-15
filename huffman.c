#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "limits.h"
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
void encode_chars(Node* node, char coded_arr[256][128], char* code, int* node_count) {
    
    if(node->c != '\0'){
        strcpy(coded_arr[(unsigned char)node->c], code);
        if(node_count != NULL) (*node_count)++;
        return;
    }

    char left_code[128];
    char right_code[128];

    strcpy(left_code, code);
    strcat(left_code, "0");
    encode_chars(node->left, coded_arr, left_code, node_count);

    strcpy(right_code, code);
    strcat(right_code, "1");
    encode_chars(node->right, coded_arr, right_code, node_count);

    if(node_count != NULL) (*node_count)++;
}

void print_coded(Node* root){

    char coded_arr[256][128] = {0};

    encode_chars(root, coded_arr, "", NULL);

    for(int i = 0; i < 256; i++){
        if(coded_arr[i][0] != '\0'){
            printf("%c : %s\n", (char)i, coded_arr[i]);
        }
    }
}

// Turn char* bits into unsigned char by bitwise shifting
unsigned char pack_char(char* bits){
    unsigned char dst = 0;

    
    for(int i = 0; i < CHAR_BIT; i++){
        if(bits[i] == '1'){
            dst = (dst << 1) | (unsigned char)1;
        }
        else if(bits[i] == '0'){
            dst = (dst << 1);
        }
        else{
            dst <<= (CHAR_BIT - i);
            break;
        }
    }
    // Debugging prints
    //printf("current pos: %s\n", bits);
    //printf("returning char: %02x\n", (unsigned char)dst);
    return dst;
}

// Take char* encoded with encoded text and pack it into char* packed with chars representing encoded bits
char* pack_text(char* encoded, size_t* out_len){

    // +1 for partially filled last char
    size_t packed_length = ((strlen(encoded) + (CHAR_BIT-1)) / CHAR_BIT);

    printf("packed len: %ld\n", packed_length);

    char* packed = malloc(packed_length);
    if(!packed) return NULL;

    for(size_t i = 0; i < packed_length; i++){
        packed[i] = pack_char(encoded);
        encoded += CHAR_BIT;
    }

    if(out_len != NULL) (*out_len) = packed_length;

    return packed;
}


// Append encoded chars from the user's text
char* encode_text(Node* root, char* str, int* node_count){

    // fill coded_arr with encoded version of char at arr[char]
    char coded_arr[256][128] = {0};
    encode_chars(root, coded_arr, "", node_count);

    // count bits with encoded chars
    size_t bits_count = 0;
    char* cur_char = str;
    while(*cur_char != '\0'){
        bits_count += strlen(coded_arr[(unsigned char)(*cur_char)]);
        cur_char++;
    }

    printf("\nbits: %ld\n", bits_count);

    cur_char = str;
    char* encoded = malloc(bits_count + 1);
    if(!encoded) return NULL;
    encoded[0] = '\0';

    while(*cur_char != '\0'){
        strcat(encoded, coded_arr[(unsigned char)(*cur_char)]);
        cur_char++;
    }

    return encoded;
}


// Recursively create a serialized tree 0 - internal node, 1 - leaf node
// Save the ser. tree in char* serialized in DFS sequence
void serialize_tree(Node* node, char* serialized){
    if(!node) return;

    if(node->c != '\0'){

        int ascii = (unsigned char)node->c;

        // Turn char into binary representation X -> 0bxxxxxxxx
        char bits[CHAR_BIT + 2];
        bits[0] = '1';
        for (int i = CHAR_BIT - 1; i >= 0; i--)
            bits[CHAR_BIT - i] = ((ascii >> i) & 1) ? '1' : '0';
        bits[CHAR_BIT + 1] = '\0';

        strcat(serialized, bits);
    }
    else{
        strcat(serialized, "0");
        serialize_tree(node->left, serialized);
        serialize_tree(node->right, serialized);
    }
}

void size_t_to_4chars(size_t n, char arr[]){
    arr[0] = (n >> 24) & 0xFF;
    arr[1] = (n >> 16) & 0xFF;
    arr[2] = (n >> 8)  & 0xFF;
    arr[3] = (n >> 0)  & 0xFF;
}

void fill_output_file(FILE* out, char* header, size_t header_len, size_t chars_num, char* body, size_t body_len){
    printf("final text:\n");
    fwrite(header, sizeof(char), header_len, out);
    char chars_num_arr[4] = {0};
    size_t_to_4chars(chars_num, chars_num_arr);
    fwrite(chars_num_arr, sizeof(char), 4, out);
    fwrite(body, sizeof(char), body_len, out);
}


// ./huffman c input.txt compressed.txt
// ./huffman e compressed.txt output.txt
int main(int argc, char* argv[]){
    char* to_encode = "AAAAABBBCCD";
    char mode = 'c'; // c - compress, e - extract
    FILE* in_file = stdin;
    FILE* out_file = stdout;


    // --- Parsing arguments ./huffman [-c|-e] [input] [output] ---
    int i = 1;

    if (argc > 1 && argv[1][0] == '-') {
        if (strcmp(argv[1], "-c") == 0) {
            mode = 'c';
        } else if (strcmp(argv[1], "-e") == 0) {
            mode = 'e';
        } else {
            fprintf(stderr, "unknown flag: %s\n", argv[1]);
            fprintf(stderr, "usage: ./huffman [-c|-e] [input] [output]\n");
            return 1;
        }
        i++;  // move past the flag
    }

    // check for input file
    if (i < argc) {
    if (strcmp(argv[i], "-") == 0) {
        in_file = stdin;  // explicit stdin
    } else {
        in_file = fopen(argv[i], "rb");
        if (!in_file) {
            fprintf(stderr, "could not open input file: %s\n", argv[i]);
            return 1;
        }
    }
    i++;
    }

    // check for output file
    if (i < argc) {
        out_file = fopen(argv[i], "wb+");
        if (!out_file) {
            fprintf(stderr, "could not open output file: %s\n", argv[i]);
            if (in_file != stdin) fclose(in_file);
            return 1;
        }
    }

    if(mode == 'e') return 0; 

    Node* root = create_huffman_tree(to_encode);
    print_coded(root); // prints code for each char

    int node_count = 0;
    char* encoded = encode_text(root, to_encode, &node_count);
    printf("encoded: %s\n", encoded);

    size_t body_len = 0;
    char* packed_body = pack_text(encoded, &body_len);
    free(encoded);
    printf("packed: %s\n", packed_body);

    char* serialized = malloc(node_count * 9);
    serialized[0] = '\0';
    serialize_tree(root, serialized);
    printf("\nserialized: %s\n", serialized);
    size_t header_len = 0;
    char* packed_header = pack_text(serialized, &header_len);
    printf("packed header: %s\n", packed_header);
    
    fill_output_file(out_file, packed_header, header_len, strlen(to_encode), packed_body, body_len);
    
    free(packed_header);
    free(packed_body);
    free_tree(root);

    return 1;
}