# SHAwords

## About
*Author: Eirik Vesterkjaer, 2018 ([eirikeve @ github](https://www.github.com/eirikeve))*

SHAWords hashes SHA keys of any length (like `git` commit hashes, and hex values in general) to words. The same input will always result in the same output.
Note that several SHA keys can hash to the same sets of words, as the lists of words are limited. 
The lists of words are from the 1934 Webster's 2nd International dictionary (copyright elapsed - [link to source here](https://svnweb.freebsd.org/csrg/share/dict/).)

## Installation
There are no dependencies except for the standard libraries `<string>`, `<iostream>`, `<algorithm>`,  and `<sstream>`. 
You can build with your `C++` compiler, and then rename/add the executable to path.
```
me@my-computer:~/path$ clang++ shawords.cpp words.hpp
```




## Usage

**Input**: hex value of any length.  
**Output**: `roundup(number of characters in input / 10)` words. 
SHAwords can evaluate arguments from `argv`, and piped arguments.


Here's 10 a char input hashed into a word:
```
me@my-computer:~$ shawords 1111122222
captivate 
```
Here's 40 chars, starting with the same 10 chars:
```
me@my-computer:~$ shawords 1111122222aaaaabbbbb2222244444eeeee
captivate people laurie wildfire
```
Another example, where the input is 33 chars. The word `parent` is the hash of `5af` + `fde6adb` (last three chars + first seven chars), using the "normal word" table (see **How it hashes** below).
```
me@my-computer:~$ shawords fde6adba660d85b6e14848451db1e35af
spree should tracy parent 

```
Evaluating `git` commit hashes as arguments, and then with piping:
```
me@my-computer:~/git-repo$ shawords $(git rev-parse HEAD)
ethane by piet matchmake 
```
```
me@my-computer:~/git-repo$ (git rev-parse HEAD) | shawords
ethane by piet matchmake 
```

**Note**: If you get `a`, `the`, or `aaron` - the first elements of the word tables - as outputs, it's probably because your input was not hexadecimal, and the conversion then gives a value of 0.

## How it hashes
Each 10 characters from the input will hash to one word. If the input is not divisible by 10, it is padded using the characters from the beginning.

This means that `shawords 2018` produces the same output as `shawords 2018201820`: `thirtyfold`.

Each of these chunks of 10 characters is interpreted as a hex number and converted to decimal, and then used as an index for selecting a word. The indexes wrap around/overflow, so several values *can* hash to the same word.
If you want to change the number of chars used for each word, edit the value of `CHAR_CHUNK_SIZE`.


The words come from three different tables in [`words.hpp`](words.hpp), so the same sequence of characters can result in different words depending on their placement in the input string. Still, the same hex value input will *always* result in the same word sequence.
* The **first word** is a normal word from Webster's dictionary (25487 entries).  
* The **second word** is from their list of connective words (150 entries).  
* The **third word** is a name from their list of proper names (1323 entries). 
* All **subsequent words** are normal words, like the first one.  

## License

MIT license, see [`LICENSE`](./LICENSE).


