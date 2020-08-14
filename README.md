# Arabic Spelling Correction
Implementation of: Linear Spelling check, BK-Tree, and SymSpell spelling checking algorithms on Arabic dictionary, in C++

The code mainly implements:
* Levenshtein distance
* Linear Spell Checking
* BK-Tree Spel Checking
* SymSpell Spell Checking

### I 'll be back to:
* Add better documentation and how-to-use
* Currently the code only works for edit distances = 0,1,2,3 only. I will add a more general code.
* Add classes instead of this code spaghetti :'D
* Add Levenshtein distance filtering to enable Levenshtein-only distance in the Symspell Algorithm (instead of Damareu-Levenshtein default).

### Possible improvements:
* Use static array (instead of vector) for suggested words -- with maximum number of suggested words
* Use static array of bytes instead of string.

### Resources:
* https://www.geeksforgeeks.org/bk-tree-introduction-implementation/
* https://towardsdatascience.com/symspell-vs-bk-tree-100x-faster-fuzzy-string-search-spell-checking-c4f10d80a078

-------------------------------------
If you need to use the code and couldn't, please contact me: wmustafaawad@gmail.com
