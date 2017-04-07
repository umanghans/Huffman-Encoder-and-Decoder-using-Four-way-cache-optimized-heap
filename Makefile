all :
	g++ encoder.cpp input_reader.cpp huffman_four_way_cache_heap.cpp -std=c++11 -o encoder
	g++ decoder.cpp -std=c++11 -o decoder

