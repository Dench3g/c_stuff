typedef struct Dict Dict;
typedef struct Bucket Bucket;
typedef struct Entry Entry;

typedef int (*EqualsFunction)(void*, void*);
typedef int (*HashCodeFunction)(void *);


struct Dict {
    HashCodeFunction hashCode;
    EqualsFunction equals;
    int size;
    struct Bucket *buckets;
};



struct Entry {
    Entry *next;
    void *key;
    void *value;
};



struct Bucket {
    Entry root;
};


void dict_init(Dict*, EqualsFunction, HashCodeFunction);
void *dict_get(Dict*, void*);
void *dict_set(Dict*, void*, void*);
void *dict_remove(Dict*, void*);
