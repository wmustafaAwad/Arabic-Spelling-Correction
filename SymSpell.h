#pragma once
#include "proj_utils.h"


/**
Implements the SymSpell (Symmetric Spell) Spelling Check.

Implemented by: Waleed Mustafa. This is definetly not the most optimized  version out there. I wrote the code for educational purposes.
**/

/** This file defines the SymSpell class**/


class SymSpellUtil{

private:
	void get_word_edits_rec_ar (wstring& word, const size_t tol, const size_t base, unordered_set <wstring>& word_edits);
	void get_word_edits_in_range_ar(wstring& word, const size_t tol, const size_t base, unordered_set <wstring>& word_edits);
	

public:
	unordered_map<wstring, unordered_set<wstring>>  my_umap_ar_dict;

	void create_symspell_umap_from_dict_ar(vector<wstring>& ar_dict, int edit_distance);
	unordered_set <wstring> SymSpell_getSimilarWords_ar(wstring& s,const int tol);
	SymSpellUtil();
	

};



