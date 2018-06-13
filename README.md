# SHAwords

## About
*Author: Eirik Vesterkjaer, 2018 ([eirikeve @ github](https://www.github.com/eirikeve))*

SHAWords hashes SHA keys of any length (like `git` commit hashes, and hex values in general) to words. 
Note that several SHA keys can hash to the same sets of words, as the lists of words are limited. 
The lists of words are from the 1934 Webster's 2nd International dictionary (copyright elapsed - [link to source here](https://svnweb.freebsd.org/csrg/share/dict/).)

## Installation
There are no dependencies except for the standard libraries `<string>`, `<iostream>`, `<algorithm>`,  and `<sstream>`. 
You can build with your `C++` compiler, and then rename/add the executable to path.
```
me@my-computer:~/path$ clang++ shawords.cpp words.hpp
```




## Usage
SHAWords takes a SHA key of any length as input, and hashes it to `roundup(number of characters / 10)` words. It can evaluate arguments from `argv`, and piped arguments.

Here we have a single word:
```
me@my-computer:~$ shawords 0123456789
rhenish
```
Here we have the same word (since the first 10 chars of the input is the same), followed by other words:
```
me@my-computer:~$ shawords 0123456789012345678901234567890aaaaaaaaa
rhenish who ole abhorrent 
```
Another example:
```
me@my-computer:~$ shawords fde6adba660d85b6e14848451db1e35af012ba19
spree should tracy torpedo 
```
Evaluating a `git` commit's SHA-hash:
```
me@my-computer:~/git-repo$ shawords $(git rev-parse HEAD)
sean like amanda bass 
```

```
me@my-computer:~/git-repo$ (git rev-parse HEAD) | shawords
sean like amanda bass 
```

## How it hashes
Each 10 characters from the input will hash to one word. If the input is not divisible by 10, it is padded using the characters from the beginning.

This means that `shawords 2018` produces the same output as `shawords 2018201820`: `thirtyfold`.

Each of these chunks of 10 characters is interpreted as a hex number and converted to decimal, and then used as an index for selecting a word. The indexes wrap around/overflow, so several values *can* hash to the same word.
If you want to change the number of chars used for each word, edit the value of `CHAR_CHUNK_SIZE`.


When it comes to the output:
* The **first word** is a normal word from Webster's dictionary (25487 entries).  
* The **second word** is from their list of connective words (1323 entries).  
* The **third word** is a name from their list of proper names (150 entries). All **subsequent words** are normal words, like the first one.  


## License

MIT license, see [`LICENSE`](./LICENSE).


