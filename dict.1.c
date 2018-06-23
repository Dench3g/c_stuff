#include "dict.1.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void dict_init(Dict* dict, EqualsFunction equals, HashCodeFunction hashCode, int bucket_size) {
    dict->buckets = calloc(bucket_size, sizeof(Entry));
    dict->size = bucket_size;

    for (int i = 0; i < bucket_size - 1; ++i) {
        dict->buckets[i].next = &dict->buckets[i + 1];
    }
    dict->buckets[bucket_size - 1].next = NULL;

    for (int i = 0; i < bucket_size; ++i) {
        dict->buckets[i].key = NULL;
    }

    dict->equals = equals;
    dict->hashCode = hashCode;
};

Entry *dict_get_bucket(Dict *dict, int hash) {
    int index = hash % dict->size;
    Entry *bucket = dict->buckets + index;
    return bucket;
};

Entry *dict_get_entry(Dict *dict, Entry *bucket, void *key, int hash) {
    for (Entry *e = bucket->next; e && e->key; e = e->next) {
        if (dict->hashCode(e->key) == hash && dict->equals(e->key, key)) {
            return e;
        }
    }
    return NULL;
};

Entry *dict_add_entry(Entry *bucket) {
    Entry *new_entry = malloc(sizeof(Entry));
    new_entry->next = bucket->next;
    bucket->next = new_entry;
    return new_entry;
};


void *dict_remove_entry(Dict *dict, Entry *bucket, void *key, int hash) {
    for(Entry *e = bucket; e->next && e->next->key; e = e->next) {
        Entry *candidate = e->next;

        if (dict->hashCode(candidate->key) == hash && dict->equals(candidate->key, key)) {
            e->next = candidate->next;
            void *result = candidate->value;
            free(candidate);
            return result;
        }
    }

    return NULL;
};



void dict_set(Dict *dict, void *key, void *value) {
    int hash = dict->hashCode(key);
    Entry *bucket = dict_get_bucket(dict, hash);
    Entry *e = dict_get_entry(dict, bucket, key, hash);
       
    if (!e) {
        e = dict_add_entry(bucket);
        e->key = key;
    }
    e->value = value;

};

void *dict_remove(Dict *dict, void *key) {
    int hash = dict->hashCode(key);
    Entry *bucket = dict_get_bucket(dict, hash);
    void *value = dict_remove_entry(dict, bucket, key, hash);
    return value;
};


void *dict_get(Dict *dict, void *key) {
    int hash = dict->hashCode(key);
    Entry *bucket = dict_get_bucket(dict, hash);
    Entry *e = dict_get_entry(dict, bucket, key, hash);
    return e->value;
}


void dict_free(Dict *dict) {
    Entry *buckets = dict->buckets;
    Entry *e = buckets->next;
    while (e) {
        Entry *candidate = e;
        e = e->next;
        if (candidate->key) {
            free(candidate);
        }
    }
    free(dict->buckets);
}

Entry *first(Dict *dict) {
    return next(dict->buckets);
}

Entry *next(Entry *entry) {
    for (Entry *e = entry->next; e; e = e->next) {
        if (e->key) {
            return e;
        }
    }
    return NULL;
}


int stringEquals(void *s1, void *s2) {
    return strcmp((char *)s1, (char *)s2) == 0;
};

int strHash(void *s) {
    char *str  = (char *)s;
    return strlen(str);
};


int main(int argc, char **argv) {
    Dict dict;
    dict_init(&dict, stringEquals, strHash, 20);

    dict_set(&dict, "Key", "Value");
    dict_set(&dict, "Hello", "World");

    // char *s = (char*)dict_get(&dict, "Key");
    // printf("KEY: %s\n", s);

    for (Entry *e = first(&dict); e; e = next(e)) {
        printf("KEY: %s\n", (char *)e->key);
        printf("VALUE: %s\n", (char *)e->value);
    }


    dict_remove(&dict, "Key");


    for (Entry *e = first(&dict); e; e = next(e)) {
        printf("KEY: %s\n", (char *)e->key);
        printf("VALUE: %s\n", (char *)e->value);
    }


    dict_free(&dict);

};