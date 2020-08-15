#include "LinSpell.h"

/* LinSpell Utilities */

vector <wstring> LinSpell_getSimilarWords_ar(wstring& word, vector <wstring> ar_dict, const int tol)  // vectors are slow. might change later?
{ 
    vector < wstring > ret; // to store candidates

	for (wstring compared_word: ar_dict) //just loop over all words in the vector and compare them with the input word
		{
			if(editDistance_ar(word, compared_word) <= tol){
				ret.push_back(compared_word);
			}
	}
     
    return ret; 
} 

