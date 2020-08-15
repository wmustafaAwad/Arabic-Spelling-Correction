#pragma once
#include "proj_utils.h"

/**
Implements the Linear Spelling Check.

	Using the edit_distance from proj_utils, loops over all words in a dictionary and compares them to
an input word. If distance <= TOL : the word is added to the returned set of words.

**/

/** This file does not define a class **/

vector <wstring> LinSpell_getSimilarWords_ar(wstring& word, vector <wstring> ar_dict, const int tol);