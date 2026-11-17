#ifndef HASHMAP_H
#define HASHMAP_H

typedef struct Value Value;

typedef struct {
    const char* key;
    Value* value;
}KeyPair;

typedef struct {
    KeyPair* key_pairs;
}HashMap;

void initialize_hashmap(HashMap* map);
bool put(KeyPair* key_pair);
bool remove(const char* key);
Value* get(const char* key);
bool exists(const char* key);
void free_hashmap(HashMap* map);

#endif
