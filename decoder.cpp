#include <iostream>
#include <bitset>
#include <unordered_map>
#include <fstream>
#include <string>
#include <cstdlib>
#include <limits.h>
#include "decoder.h"

using namespace std;
struct bt_node *btr = NULL;

void build_decode_tree()
{
    btr = (struct bt_node *) malloc(sizeof(struct bt_node ));
    btr->data = INT_MAX;
    btr->left = NULL;
    btr->right = NULL;
}

void insert_decode_tree(int key, string code)
{
    struct bt_node *ptr = btr;
    for(int i = 0; i < code.size(); i++) {
        if(code[i] == '0' ) {
            if( ptr->left == NULL) {
                ptr->left = (struct bt_node *) malloc(sizeof(struct bt_node ));
                ptr->left->data = INT_MAX;
                ptr->left->left = NULL;
                ptr->left->right = NULL;
                ptr->data = INT_MAX;
            }
            ptr = ptr->left;
        }

        if(code[i] == '1' ) {
            if(ptr->right ==  NULL) {
                ptr->right = (struct bt_node *) malloc(sizeof(struct bt_node ));
                ptr->right->data = INT_MAX;
                ptr->right->left = NULL;
                ptr->right->right = NULL;
                ptr->data = INT_MAX;
            }
            ptr = ptr->right;
        }

    }
    ptr->data = key;

}

void generate_output_table(string &encoded_string)
{
    int index = 0;
    ofstream output_table;
    output_table.open("decoded.txt");

    struct bt_node *ptr = btr;

    for(int i = 0; i < encoded_string.size(); i++) {
        if(encoded_string[i] == '0') {
            ptr = ptr->left;
        } else {
            ptr = ptr->right;
        }
        if(ptr->left == NULL && ptr->right == NULL) {
            output_table << ptr->data;
            ptr = btr;
            output_table << "\n";
        }
    }

    output_table.close();
}

void print_tree(struct bt_node *ptr)
{
    if(ptr == NULL)
        return;
    print_tree(ptr->left);
    print_tree(ptr->right);

}

void generate_decode_tree(string &encoded_file_name, string &code_table_file_name, string &encoded_string)
{
    unordered_map<int, string> code_table;
    ifstream code_table_file;
    ifstream encoded_file;
    code_table_file.open(code_table_file_name.c_str(), ios::in);
    unsigned char temp;

    FILE *input;
    input = fopen(encoded_file_name.c_str(), "rb");

    while(fread((unsigned char *)&temp,  sizeof(char), 1, input)) {
        bitset<8> bs(temp);
        encoded_string += bs.to_string();
    }

    fclose(input);

    int temp_val;
    string temp_code;
    while(!code_table_file.eof()) {
        code_table_file >> temp_val;
        code_table_file >> temp_code;
        code_table[temp_val] = temp_code;
    }

    code_table_file.close();

    build_decode_tree();

    for(auto i = code_table.begin(); i != code_table.end(); i++ ) {
        insert_decode_tree(i->first, i->second);
    }

    print_tree(btr);

}

void decode_huffman(string &encoded_file, string &code_table) {
    string encoded_string = "";
    generate_decode_tree(encoded_file, code_table, encoded_string);
    generate_output_table(encoded_string);
}

int main(int argc, char *argv[])
{
    if(argc != 3) {
        cout << "Incorrect usage. Correct usage : " << argv[0] << " encoded_file.bin code_table.txt\n ";
    } else {
        string encoded_file(argv[1]);
        string code_table(argv[2]);

        decode_huffman(encoded_file, code_table);
    }
}
