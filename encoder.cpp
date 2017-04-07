#include <iostream>
#include <bitset>
#include <string>
#include <fstream>
#include "input_reader.h"
#include "huffman_four_way_cache_heap.h"
#include "input_reader.h"

using namespace std;

void traverse_huffman_tree_four_way(struct min_queue *mq, ofstream &output_code_table, string aux_array, unordered_map<int, string> &code_table)
{
    if(mq->left == NULL && mq->right == NULL) {
        output_code_table << mq->data << " " << aux_array << "\n";
        code_table[mq->data] = aux_array;
        return;
    }
    traverse_huffman_tree_four_way(mq->left, output_code_table, aux_array + "0", code_table);

    traverse_huffman_tree_four_way(mq->right, output_code_table, aux_array + "1", code_table);
}

void generate_code_table_four_way_heap(unordered_map<int, string> &code_table)
{
    ofstream output_code_table;
    string aux_array = "";
    output_code_table.open("code_table.txt");

    traverse_huffman_tree_four_way(&four_way_heap[3], output_code_table, aux_array, code_table);

    output_code_table.close();
}

void encode_bin_file_four_way(unordered_map<int, string> &code_table, vector<int> &input_table)
{
    FILE *output;
    output = fopen("encoded.bin", "wb");

    string hf_string = "";

    for(int i = 0; i < input_table.size(); i++) {
        hf_string += code_table[input_table[i]];
    }

   for(int i = 0; i < hf_string.size() ; i += 8) {
        bitset<8> bst(hf_string.substr(i,8));
        fwrite((unsigned char *) &bst, sizeof(char), 1, output);
    }

    fclose(output);
}

int main(int argc, char *argv[])
{
    unordered_map<int, int> freq_table;
    vector<int> input_table;
    unordered_map<int, string> code_table;

    if(argc != 2) {
        cout << "Incorrect usage. Correct usage : " << argv[0] << " input_file.txt\n ";
    } else {
        string encoded_file(argv[1]);

        f_table(encoded_file,  freq_table, input_table);

        build_tree_using_4way_heap(freq_table);

        generate_code_table_four_way_heap(code_table);
        encode_bin_file_four_way(code_table, input_table);

    }
}

