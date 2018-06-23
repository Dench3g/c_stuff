typedef struct Dict Dict;
typedef struct Entry Entry;

typedef int (*EqualsFunction)(void*, void*);
typedef int (*HashCodeFunction)(void *);


struct Dict {
    HashCodeFunction hashCode;
    EqualsFunction equals;
    int size;
    struct Entry *buckets;
};



struct Entry {
    Entry *next;
    void *key;
    void *value;
};

void dict_init(Dict *, EqualsFunction, HashCodeFunction, int);
void *dict_get(Dict *, void *);
void dict_set(Dict *, void *, void *);
void *dict_remove(Dict *, void *);

//for cleanup purposes
Entry *next(Entry *);
Entry *first(Dict *dict);
