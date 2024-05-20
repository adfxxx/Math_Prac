all: main

main: main.c structs/logger.c structs/data.c structs/department.c structs/operator.c sets/bst.c sets/dynamic_array.c sets/hash_set.c sets/set.c sets/trie.c heaps/binary_heap.c heaps/binomial_heap.c heaps/fib_heap.c heaps/heap.c heaps/leftist_heap.c heaps/skew_heap.c heaps/treap.c functions/read_file.c functions/status.c functions/validation.c
	gcc -g main.c structs/logger.c structs/data.c structs/department.c structs/operator.c sets/bst.c sets/dynamic_array.c sets/hash_set.c sets/set.c sets/trie.c heaps/binary_heap.c heaps/binomial_heap.c heaps/fib_heap.c heaps/heap.c heaps/leftist_heap.c heaps/skew_heap.c heaps/treap.c functions/read_file.c functions/status.c functions/validation.c -o main -lm

clean:
	rm -rf *.o main
