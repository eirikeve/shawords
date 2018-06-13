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

// # of chars of input used for hashing per word; 1 chunk -> 1 word
const int CHAR_CHUNK_SIZE = 10;

void shawords(const std::string &SHAkey);


int main(int argc, char *argv[]) {
    // If argv has args, evaluate them
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            shawords(std::string(argv[i]));
        }

    // If nothing in argv, try to evaluate cin (for piping)
    } else {
        std::string input;
        while (std::cin >> input) { 
            shawords(input);
        }
    }
    return 0;
}


/*
convertToFullChunkString pads the end of SHAkey using its entries and returns it as a string.
@arg SHAkey: hex value as str, of any length
@arg chunkLenght: SHAkey will be padded so it is length n * chunkLength
@return: padded SHAkey
*/
std::string convertToFullChunkString(const std::string &SHAkey, const int &chunkLength) {
    int len = SHAkey.length();

    // If it isn't, make the SHA-key into full chunks
    if ((len % chunkLength) != 0) {

        // Rounded up to nearest n * chunkLength
        int len_new = len + (chunkLength - (len % chunkLength));
        std::string SHAkey_new = std::string(SHAkey);
        for (int i = len; i < len_new; i++) {
            SHAkey_new = SHAkey_new + SHAkey[i % len];
        }
        SHAkey_new = SHAkey_new + "\0";
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

@arg hexValue: of any size, is treated as unsigned
@arg wordType: word (0), connective (1), or proper name (2), see words.hpp
@return: word
*/
std::string hashToWord(const std::string &hexValue, const int &wordType) {
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
@arg SHAkey: hex value of any length
*/
void shawords(const std::string &SHAkey){
    try {
        std::string SHAkey_padded = convertToFullChunkString(SHAkey, CHAR_CHUNK_SIZE);
        int chunks = SHAkey_padded.length() / CHAR_CHUNK_SIZE;
        std::string words[chunks];

        for (int i = 0; i < chunks; i++) {
            // Split into chunks, convert, and output
            std::string hexValue = SHAkey_padded.substr(i*CHAR_CHUNK_SIZE, CHAR_CHUNK_SIZE);
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
