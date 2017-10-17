#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "AnagramDictionary.h"
using namespace std;

//#define DEBUG_HASH_TABLE 1

// prime number array for computinh hash for given word.
unsigned char primes[26] = {5,71,79,19,2,83,31,43,11,53,37,23,41,3,13,73,101,17,29,7,59,47,61,97,89,67};

static void trimTrailing(char * str);

HashVal AnagramDictionary::hashWord (const char *str, size_t len)
{
    size_t idx;
    HashVal val=1;

    if (!len) {
        return 0;
    }

    for (idx = 0; idx < len; idx++) {
        char ch = str[idx];
        if (ch >= 'A' && ch <= 'Z' ) {
            val *= primes[ ch - 'A'];
        } else if (ch >= 'a' && ch <= 'z' ) {
            val *= primes[ ch - 'a'];
        } else {
            continue;
        }
    }
    return val;
}

struct hashEntry *AnagramDictionary::newWord(char *str, size_t len)
{
    struct hashEntry *entry;
    if (!len) {
        len = strlen(str);
    }
    entry = (hashEntry*) malloc(len + sizeof *entry );
    entry->hash = hashWord(str, len);
    entry->next = NULL;
    strncpy (entry->word, str, len);
    entry->word[len] = 0;
    return entry;
}

void AnagramDictionary::hashAddEntry(struct hashEntry *entry)
{
    unsigned int slot;
    struct hashEntry *tmp,*prev;
    slot = entry->hash % HASHTABLE_SIZE;

    prev = NULL;
    tmp = hashtab[slot];
    while (tmp != NULL) {
      prev = tmp;
      tmp = tmp->next;
    }
    if (prev != NULL) {
       prev->next = entry;
    } else {
       hashtab[slot] = entry;
    }
    entry->next = NULL;
}

#define BUFSIZE 1024

    /* Build the hash table */
void AnagramDictionary::buildHashTable() {
    size_t pos,end;
    struct hashEntry *wp, **pp;
    HashVal val;
    // allocate the hashtable in heap.
    hashtab = new struct hashEntry* [HASHTABLE_SIZE];
    // initialize with zero.
    memset(hashtab, 0, HASHENTRY_PTR_SIZE * HASHTABLE_SIZE);
    // temporary buffer size
    char buf[BUFSIZE];
    char* buff = buf;

    int cnt =0;
    int len;
    // get the next word from dictionary file.
    while (fgets(buff, BUFSIZE, fp)) 
    {
        cnt++;
        trimTrailing(buff);
        len = strlen(buff);

        // allocate a new hash table entry.
        wp = newWord(buff,len );

   #ifdef DEBUG_HASH_TABLE
        if ( (cnt % 10000) == 0) {
            printf("cnt: %d, str: %s, Slot %llu \n", cnt, buff, wp->hash);
        }
   #endif
        hashAddEntry(wp);
    }
}

/* Dump the hash table */
void AnagramDictionary::dumpHashTable() {
    size_t pos;
    struct hashEntry* tmp;
    HashVal val;

    for (pos = 0;  pos < HASHTABLE_SIZE; pos++) 
    {
        if (!hashtab[pos]) {
            continue;
        }

        printf("\nSlot:%lu:\n", pos);
        tmp =  hashtab[pos];
        while (tmp != NULL) {
            printf("\t%llx:%s", tmp->hash, tmp->word);
            tmp = tmp->next;
        }
    }
}

void AnagramDictionary::deleteHashTable() {
    size_t pos;
    struct hashEntry *tmp, *next;

    for (pos = 0;  pos < HASHTABLE_SIZE; pos++) 
    {
        if (hashtab[pos] == NULL) {
            continue;
        }

        next =  hashtab[pos];
        while (next != NULL) {
            tmp = next->next;
            free(next);
            next = tmp;
        }
    }
    if (hashtab != NULL) {
        delete[] hashtab;
    }
}

AnagramDictionary::AnagramDictionary(const char* filename) {
    fp = fopen(filename,"r");
    if (fp == NULL) {
        printf("Can't open file \n");
        return;
    }
    buildHashTable();
}

AnagramDictionary::~AnagramDictionary() {
    deleteHashTable();
    if (fp != NULL) {
        fclose(fp);
    }
}

const char* AnagramDictionary::findFirstAngaramWord(const char* word) {
    const char* anagWord = NULL;
    HashVal val = hashWord(word, strlen(word));
    unsigned int slot = val % HASHTABLE_SIZE;
    hashEntry* entry = hashtab[slot];
    if (entry == NULL) {
        return NULL;
    } else {
        for (;entry != NULL; entry = entry->next) {
            if (entry->hash == val) {
                anagWord = entry->word;
                break;
            }
            
        }
    }
    return anagWord;
}

vector<string> AnagramDictionary::findAllAnagramWords(const char* word) {
    vector<string> words;
    HashVal val = hashWord(word, strlen(word));
    unsigned int slot = val % HASHTABLE_SIZE;
    hashEntry* entry = hashtab[slot];
    if (entry != NULL) {
        for (;entry != NULL; entry = entry->next) {
            if (entry->hash == val) {
                //anagWord = entry->word;
                words.push_back(entry->word);
            }
        }
    }
    return words;
}

void trimTrailing(char * str)
{
    int index, i;

    /* Find last index of non-white space character */
    i = 0;
    while(str[i] != '\0') {
        if(str[i] != ' ' && str[i] != '\t' && str[i] != '\n') {
            index= i;
        }
        i++;
    }

    /* Mark next character to last non-white space character as NULL */
    str[index + 1] = '\0';
}

