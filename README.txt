                    Hashtable based Anagram Finder in a dictionary.
                    ===============================================
                    
 Goal:
 
    A simple tool for looking up anagrams of a given string in a dictionary of words.
    

 Usage:
 
    AngramDictionary <dictionary> <word> [all] 
         Required: <dictionary> file containing all words in dictionary 
         Required: <word> word to find the matching anagram(s) from dictionary 
         Optional: [all] is to print all matching anagram words from dictionary 
         
         
 Algorithm:
 
     The algorithm for building the hash table for dictionary:

         1) Define a prime number array for each ASCII char based on the common frequency of it's usage in English language.         
                unsigned char primes[26] = {5,71,79,19,2,83,31,43,11,53,37,23,41,3,13,73,101,17,29,7,59,47,61,97,89,67};                
            This is used in hash table key calculation as given below.

         2) For each word in dictionary do this:
              a)  For each character in the given word do this:
                      i) Find a representative prime number for each alphabet char by looking up it's value from primes[] array.  For example, for 'a', look up from [0] i.e 5.

              b) Multiply all the prime numbers looked up from the above step to create the unique hash value.
              Note: Why prime number instead of just it's ASCII value? multiplying prime numbers gives us unique hash values since there are no common factors between prime numbers.

              c) Find a slot in hash table to store it .
                       slot =  HashVal  % [HASHTAB_SIZE]

         3) Once the hash table is built, the string match can be done in O(1) time by simply looking up in the hash table if there is a match found. It does take some time to calculate the HashVal for the given string, but it should be comparatively much smaller than looking up all the values in dictionary.
         
         
 Sample Runs and Timings:
 
        ~/myproj/AnagramDictionary/Debug>time ./AngramDictionary ~/myproj/english.txt mean all
        First Found match: amen for mean 
        Matching words:
                amen
                mane
                mean
                name

        Average lookup time for random strings: in 1000 iterations is: 5.696 micro seconds

        real    0m0.102s
        user    0m0.092s
        sys     0m0.012s
        
        ~/myproj/AnagramDictionary/Debug>time ./AngramDictionary ~/myproj/english.txt miscreation
        First Found match: creationism for miscreation 
        Average lookup time for random strings: in 1000 iterations is: 5.804 micro seconds

        real    0m0.103s
        user    0m0.096s
        sys     0m0.004s
        
        
        ~/myproj/AnagramDictionary/Debug>time ./AngramDictionary ~/myproj/english.txt tan
        First Found match: ant for tan 
        Average lookup time for random strings: in 1000 iterations is: 5.647 micro seconds

        real    0m0.097s
        user    0m0.092s
        sys     0m0.008s

   On average it takes about 5 micro seconds to lookup a random string in 1000 iterations.


                    
