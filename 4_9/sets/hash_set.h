#include "set.h"

typedef struct Hash_elem{
    int key;
    Department *dep;
    struct Hash_elem *next;
    int next_count;
}Hash_elem;

typedef struct Hash_set{
    int size;
    Hash_elem **elems;
    int max_size;
    int min_size;
}Hash_set;

Hash_set *create_hash();
Status insert_hash(Hash_set *set, int key, Department *dep);
Status search_hash(Hash_set *set, int key, Department **res);
void free_hash(Hash_set *set);

Hash_elem **create_hash_elem(int size);
Hash_elem *create_hash_elem_in(int key, Department *dep);
void free_hash_elems(Hash_elem **elems, int size);
void free_hash_elem_in(Hash_elem *elem);
Status rebuild_hash(Hash_set *set);
void low_chain_size(Hash_elem *from, Hash_elem *to);
Status change_elem_size_hash(Hash_elem ***elems, int old_size, int new_size);
int get_prime_num(int size);
int hash_function(int key, int size);