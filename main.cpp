#include <string>
#include <iostream>
#include <unistd.h>
#include "string.h"

#include "AnagramDictionary.h"
using namespace std;
char *progName;


void printUsage(const char* error) {
    if (error) {
        printf("ERROR: %s \n", error);
    }
    printf("Usage: %s <dictionary> <word> [all] \n", progName);
    printf("\t Required: <dictionary> file containing all words in dictionary \n");
    printf("\t Required: <word> word to find the matching anagram(s) from dictionary \n");
    printf("\t Optional: [all] is to print all matching anagram words from dictionary \n");
}

bool checkFile(char* filename) {
    return (access( filename, F_OK ) != -1 );
}

int main(int argc, char* argv[])
{
    bool matchAll = false;
    progName = argv[0];

    if (argc < 3) {
        printUsage(NULL);
        exit(1);
    } else if (!checkFile(argv[1])) {
        printUsage("given dictionary file does not exist");
    }
    if (argc >=4 && strcmp(argv[3],"all")==0) {
        matchAll = true;
    }
    const char *fileName = argv[1];
    const char *word = argv[2];

    AnagramDictionary dict(fileName);
    //dict.dumpHashTable();

    const char* matchWord = dict.findFirstAngaramWord(word);
    if (matchWord != NULL) {
        printf("First Found match: %s for %s \n", matchWord, word);
    } else {
        printf("No match for: %s \n", word);
    }

    if (matchAll && matchWord) {
        vector<string> words = dict.findAllAnagramWords(word);
        if ((words.size() >1)) {
            printf("Matching words:\n   ");
            for (int i=0; i< words.size(); i++) {
                cout<< '\t' << words[i] << endl;
            }
            cout<< "\n";
        }
    }

    return 0;
}
