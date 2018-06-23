#include "dict.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void dict_init(Dict* dict, EqualsFunction equals, HashCodeFunction hashCode) {
    dict->buckets = calloc(20, sizeof(Bucket));
    dict->size = 20;
    dict->equals = equals;
    dict->hashCode = hashCode;
};

Bucket *dict_get_bucket(Dict *dict, void *key) {
    int hash = dict->hashCode(key);
    int index = hash % dict->size;
    Bucket *bucket = dict->buckets + index;
    return bucket;
};

Entry *dict_get_entry(Dict *dict, void *key) {

    Bucket *bucket = dict_get_bucket(dict, key);

    Entry *e = bucket->root.next;


    while (e) {

        if (dict->hashCode(e->key) == dict->hashCode(key)) {
            if (dict->equals(e->key, key)) {
                return e;
            }
        }
        e = e->next;
    }

    return NULL;
};

Entry *dict_add_entry(Bucket *bucket, void *key, void *value) {
    Entry *root = &bucket->root;
    Entry *new_entry = malloc(sizeof(Entry));
    new_entry->key = key;
    new_entry->value = value;
    new_entry->next = root->next;
    root->next = new_entry;
    return new_entry;
};

Entry *dict_remove_entry(Dict *dict, Entry *entry) {

    Bucket *bucket = dict_get_bucket(dict, entry->key);

    Entry *e = &bucket->root;

    for(;;e = e->next) {
        if (e->next == entry) {
            e->next = entry->next;
            return entry;
        }
    }
};



void *dict_set(Dict *dict, void *key, void *value) {
    Entry *e = dict_get_entry(dict, key);
    if (e) {
        e->value = value;
    } else {
        Bucket *bucket = dict_get_bucket(dict, key);
        Entry *entry = dict_add_entry(bucket, key, value);
        return entry;
    }
    return NULL;
};

void *dict_remove(Dict *dict, void *key) {
    Entry *entry = dict_get_entry(dict, entry);
    if (entry) {
        entry = dict_remove_entry(dict, entry);
        return entry;
    }
    return NULL;
};


void *dict_get(Dict *dict, void *key) {
    Entry *e = dict_get_entry(dict, key);
    return (e ? e->value : NULL);
}


void dict_free(Dict *dict) {
    for (int i = 0; i < dict->size; ++i) {
        Bucket *buckets = dict->buckets;
        Entry *e = buckets[i].root.next;
        
        while (e) {
            Entry *to_del = e;
            e = e->next;
            free(e);
        }

    }
    free(dict->buckets);
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
    dict_init(&dict, stringEquals, strHash);

    dict_set(&dict, "KEY", "VALUE");
    char *s = (char*)dict_get(&dict, "KEY");

    dict_set(&dict, "KEY", "VALUE2");
    s = (char*)dict_get(&dict, "KEY");
    printf(s);

    dict_set(&dict, "KEY2", "VALUE3");
    s = (char*)dict_get(&dict, "KEY2");
    printf(s);

    dict_free(&dict);

};