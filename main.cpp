#include <string>
#include <iostream>
#include <unistd.h>
#include "string.h"
#include <chrono>

#include "AnagramDictionary.h"
using namespace std;
using namespace std::chrono;

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

string genRandomString(int len)
{
   string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
   string newstr;
   int pos;
   while(newstr.size() != len) {
    pos = ((rand() % (str.size() - 1)));
    newstr += str.substr(pos,1);
   }
   return newstr;
}


#define LOOKUP_TIMING_TEST 1
#define LOOKUP_TIMING_TEST_ITERATIONS 1000
//#define LOOKUP_TIMING_TEST_PRINT_INDIVIDUAL_TIMING

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

#ifdef LOOKUP_TIMING_TEST
    int64_t total_duration = 0;
    // Test looking up random strings to check the average lookup speed
    srand(time(0));
    for (int i=0; i< LOOKUP_TIMING_TEST_ITERATIONS; i++) {
        string mystr = genRandomString(i+10);

        // measure the lookup time.
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        const char* str = dict.findFirstAngaramWord(mystr.c_str());
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>( t2 - t1 ).count();
        total_duration += duration;

#ifdef LOOKUP_TIMING_TEST_PRINT_INDIVIDUAL_TIMING
        if (str != NULL) {
            string matchWord(str);
            cout << "First Found match: " << matchWord  << " for " << word <<  " took " << duration << "microseconds" << endl;
        } else {
            cout << "No match for: " << mystr << " took " << duration << "microseconds" << endl;
        }
#endif
    }

    double avgtime = (double)total_duration/ (double)LOOKUP_TIMING_TEST_ITERATIONS;
    cout << "Average lookup time for random strings: in " << LOOKUP_TIMING_TEST_ITERATIONS 
        << " iterations is: " << avgtime << " micro seconds" << endl;

#endif //LOOKUP_TIMING_TEST

    return 0;
}
