#ifndef _ANAGHASH_H
#define _ANAGHASH 1

#include <string>
#include <vector>


typedef unsigned long long HashVal;

struct hashEntry {
    struct hashEntry *next;
    HashVal hash;
    char word[1];
};

#define HASHTABLE_SIZE 1048576
#define HASHENTRY_SIZE sizeof(struct hashEntry)
#define HASHENTRY_PTR_SIZE sizeof(struct hashEntry*)

class AnagramDictionary {
    protected:
        FILE* fp;
        int count;
        struct hashEntry **hashtab;
        struct hashEntry *newWord(char *str, size_t len);
        struct hashEntry **hashFind(struct hashEntry *wp);
        void hashAddEntry(struct hashEntry *entry);

        HashVal hashWord (const char *str, size_t len);
        void buildHashTable();
        void deleteHashTable();

    public:
        AnagramDictionary(const char* filename);
        virtual ~AnagramDictionary();

        void dumpHashTable();
        const char* findFirstAngaramWord(const char* word);
        std::vector<std::string> findAllAnagramWords(const char* word);

};

#endif
