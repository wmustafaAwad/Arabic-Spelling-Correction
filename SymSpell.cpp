#include "SymSpell.h"


void SymSpellUtil::get_word_edits_rec_ar (wstring& word, const size_t tol, const size_t base, unordered_set <wstring>& word_edits){
	if(( (int)(word.length()/2) - (int)tol ) <= 0){ // base case 1   // int casting from size_t to allow negative values
		word_edits.insert(L""); return;
	} 

	if( tol == 0) {// base case 2
		word_edits.insert(word); return;
	} 


	// else: go recursive
	for (size_t i = base; i<(word.length()/2); i++)
	{
		get_word_edits_rec_ar(word.substr(0,(2*i))+word.substr((2*i+2),string::npos), tol-1, i, word_edits);
	}

} 

void SymSpellUtil::get_word_edits_in_range_ar(wstring& word, const size_t tol, const size_t base, unordered_set <wstring>& word_edits){
	for (size_t i = 0; i <= tol; i++){
		get_word_edits_rec_ar(word, i, 0, word_edits);
	
	}
}

void SymSpellUtil::create_symspell_umap_from_dict_ar(vector<wstring>& ar_dict, int edit_distance){

	unordered_set<wstring> temp_uset;

	for (wstring dict_word : ar_dict){ // for word in dictionary
		get_word_edits_in_range_ar(dict_word, edit_distance, 0, temp_uset); // generate all possible edits
		for (wstring word_edit : temp_uset){ // append those edits to the set of words in the unordered map
			my_umap_ar_dict[word_edit].insert(dict_word);
		}
		temp_uset.clear();
	}
}

unordered_set <wstring> SymSpellUtil::SymSpell_getSimilarWords_ar(wstring& s,const int tol)
{
	unordered_set< wstring > possible_edits_for_word; 
	unordered_set< wstring > ret;

	get_word_edits_in_range_ar(s, tol, 0, possible_edits_for_word);

	// IMPORTANT: assumes generated umap_ar_dict was generated with equivalent edit distance

	for (wstring edited_word : possible_edits_for_word)
	{
		for (wstring x: my_umap_ar_dict[edited_word])
		{
			if(editDistance_ar(s, x) <=tol)
			{
				ret.insert(x);
			}
		}
	}

	


	return ret;
}

SymSpellUtil::SymSpellUtil(){};