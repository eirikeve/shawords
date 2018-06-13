#include "words.hpp"
#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <cstring>



/*
    About:

    SHAWords hashes SHA keys to words. 
    Several SHA keys can hash to the same sets of words, 
    as the lists of words are limited.
    See words.hpp for list of words.

    Written by Eirik Vesterkjaer, 2018 (github.com/eirikeve)
*/

// # of chars of input used for each word
const int CHAR_CHUNK_SIZE = 10;

void shawords(char * SHAkey);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Expected at least one input argument\n";
        return 1;
    }
    else {
        for (int i = 1; i < argc; i++) {
            shawords(argv[i]);
        }
    }
}


/*
convertToFullChunkString pads the end of SHAkey using its entries and returns it as a string.
*/
std::string convertToFullChunkString(char SHAkey[], int chunkLength) {
    int len = strlen(SHAkey);

    // If it isn't, make the SHA-key into full chunks
    if ((len % chunkLength) != 0) {

        // Rounded up to nearest n * chunkLength
        int len_new = len + (chunkLength - (len % chunkLength));
        char SHAkey_new[len_new];
        for (int i = 0; i < len_new; i++) {
            SHAkey_new[i] = SHAkey[i % len];
        }
        SHAkey_new[len_new] = '\0';
        return std::string(SHAkey_new);
    } else {
        return std::string(SHAkey);
    }
}


/*
hashToWord is a several-to-1 hash of hexValue & wordType to words.
Can result in a 0 index if hexValue is not actually hex, 
which doesn't cause an error, but will always give the same words as output:
(either W_WORDS[0], W_CONNECTIVES[0], or W_PROPERNAMES[0])
*/
std::string hashToWord(std::string hexValue, int wordType) {
    // Convert the hex string to a useable index
    uint64_t index;   
    std::stringstream ss;
    ss << std::hex << hexValue;
    ss >> index;
    std::string word;
    // Choose which kind of word to return
    switch (wordType) {
        case 0: // All normal words
            word = std::string(W_WORDS[index % COUNT_W_WORDS]);
            break;
        case 1: // Words like to, from, down, etc.
            word = std::string(W_CONNECTIVES[index % COUNT_W_CONNECTIVES]);
            break;
        case 2: // Names
            word = std::string(W_PROPERNAMES[index % COUNT_W_PROPERNAMES]);
            break;
        default:
            word = std::string(W_WORDS[index % COUNT_W_WORDS]);
    }
    // Lowercase
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    return word;
}

/*
shawords converts a hex number of any length to a series of roundup(len(SHAkey)/10) words
A normal SHA1 key will give 4 words
*/
void shawords(char * SHAkey){
    // Length of each sequence of the SHA to be converted to a word
    std::string SHAkey_str = convertToFullChunkString(SHAkey, CHAR_CHUNK_SIZE);
    int chunks = SHAkey_str.length() / CHAR_CHUNK_SIZE;

    try {
        std::string words[chunks];
        for (int i = 0; i < chunks; i++) {
            // Split into chunks, convert, and output
            std::string hexValue = SHAkey_str.substr(i*CHAR_CHUNK_SIZE, CHAR_CHUNK_SIZE);
            words[i] = hashToWord(hexValue, i);
        }
        for (int i = 0; i < chunks; i++) {
            std::cout << words[i] << " ";
        }
        std::cout << '\n';
    } catch (...) {
        std::cout << "Conversion failed. :-(\n";
    }
}
