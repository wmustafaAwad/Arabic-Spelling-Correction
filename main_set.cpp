// C++ program to demonstrate working of BK-Tree 
// #include "bits/stdc++.h" 
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <codecvt>
#include <cstdlib>
#include <chrono>
#include <unordered_map>
#include <unordered_set>


using namespace std; 
  
// maximum number of words in dict[] 
#define MAXN 10000 
  
// defines the tolerence value 
#define TOL  3

  
// defines maximum length of a word 
#define LEN 20  // Changed from 10 to 30 since some words had difference > 10 (eg: ائتماني )
// 20 did not work because of (eg: أعطيتمونيهم )

# define NUM_RUNS 100 
// Number of runs to average over for benchmark purposes

/* Language Utilities */
bool is_english(wstring& a){
	/*Returns true if the word is English (any letter is English), 
	False if not (interpretted as arabic in this case)*/

	//TODO: A refined version could check if all letters are arabic or all letters are English. 
	// This could help with several languages only.

	for (int i=0; i<a.length(); i++){
		int integer_wchar = (int)a[i];
		if( integer_wchar > 31 && integer_wchar < 122){ // check why these numbers: https://www.asciitable.com/
			return true;
			}
	}
	//else if a;l bytes are English character
	return false;
}

void separate_dicts(wifstream& mixed_words_wifstream, string& output_path_ar, string& output_path_en, bool print_num_words_each){
	// Read words and separate arabic and English words dict

	int eng_cntr=0, ar_cntr=0;
	std::wofstream words_ar(output_path_ar);
	std::wofstream words_en(output_path_en);
	
	std::wstring wline;



	while (std::getline(mixed_words_wifstream, wline)) // create english and arabic dictionaries
		{
			if(is_english(wline)){
				words_en << wline << endl;
				eng_cntr++;
			}else{ // is arabic
				words_ar << wline << endl;
				ar_cntr++; 
			}
		}

	if(print_num_words_each){
		cout << "We have " << eng_cntr << " English words, and " << ar_cntr << " Arabic words." << endl; 
	}
	
	words_ar.close();
	words_en.close();

}


/* Distance Utilities */

int min(int a, int b, int c) 
{ 
    return min(a, min(b, c)); 
} 

int max (int a, int b, int c){
	return max(a, max(b, c));
}

int editDistance_ar(wstring& a,wstring& b) 
{ 

    int m = a.length()/2, n = b.length()/2; 
    //wcout << "Length of " << a << " is " << m << endl; 
    //wcout << "Length of " << b << " is " << n << endl; 

    int dp[LEN+1][LEN+1]; // Changed from dynamic to static allocation. 
	// Its size does not grow anyway since it is discarded after every function run. Earlier Dynamic allocation probably caused a memory leak?
	// Can still implement dynamic allocation, but no need.

  
    // filling base cases:: comparing to empty strings, where all required is deletion
    // so the solution to the base problem = length of characters.
    for (int i=0; i<=m; i++) 
        dp[i][0] = i; 
    for (int j=0; j<=n; j++) 
        dp[0][j] = j; 
  
    // populating matrix using dp-approach 
    for (int i=1; i<=m; i++) //0 is empty string -- already filled
    { 
        for (int j=1; j<=n; j++) //0 is empty string
        { 
			if (!( ( a[2*(i-1)]==b[2*(j-1)] ) && (a[2*(i-1)+1]==b[2*(j-1)+1]) ) ) // two characters are not equal // two bytes = one character 
				// also can do: a.substr((i-1)*2,2) != b.substr((j-1)*2, 2) 
            { 
                dp[i][j] = min(dp[i-1][j],  // deletion 
                               dp[i][j-1],  // insertion 
                               dp[i-1][j-1] // replacement 
                              ) + 1; // changed +1 to be outside min --reduce flops
            } 
            else
                dp[i][j] = dp[i-1][j-1]; // sol = sol of sub-problem without those letters 
        } 
    } 

    return dp[m][n]; 
} 

int editDistance_en(string& a,string& b) 
{ 
    int m = a.length(), n = b.length(); 
    // cout << "Length of " << a << " is " << a.length() << endl; 
    // cout << "Length of " << b << " is " << b.length() << endl; 

    int dp[LEN+1][LEN+1]; // Changed from dynamic to static allocation. 
	// Its size does not grow anyway since it is discarded after every function run. Earlier Dynamic allocation probably caused a memory leak?
	// Can still implement dynamic allocation, but no need.

  
    // filling base cases:: comparing to empty strings, where all required is deletion
    // so the solution to the base problem = length of characters.
    for (int i=0; i<=m; i++) 
        dp[i][0] = i; 
    for (int j=0; j<=n; j++) 
        dp[0][j] = j; 
  
    // populating matrix using dp-approach 
    for (int i=1; i<=m; i++) //0 is empty string -- already filled
    { 
        for (int j=1; j<=n; j++) //0 is empty string
        { 
            if (a[i-1] != b[j-1]) 
            { 
                dp[i][j] = min(dp[i-1][j],  // deletion 
                               dp[i][j-1],  // insertion 
                               dp[i-1][j-1] // replacement 
                              ) + 1; // changed +1 to be outside min --reduce flops
            } 
            else
                dp[i][j] = dp[i-1][j-1]; // sol = sol of sub-problem without those letters 
        } 
    } 

    return dp[m][n]; 
} 


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



/* BK-Tree Utilities */

struct Node_ar 
{ 
    // stores the word of the current Node 
    wstring word; 
  
    // links to other Node in the tree 
    int next[2*LEN]; 
  
    // constructors 
    Node_ar(wstring x):word(x) 
    { 
        // initializing next[i] = 0 
        for(int i=0; i<2*LEN; i++) 
            next[i] = 0; 

    } 
    Node_ar() {} 
}; 

// stores the root Node of the arabic words bk-tree 
Node_ar RT_ar; 
  
// stores every Node of the arabic words bk-tree 
Node_ar tree_ar[MAXN]; 
  
// index for current Node of the arabic words bk-tree 
int ptr_ar = 0; // set to zero? --> Actually done in main.
  
// adds curr Node to the tree 
void add_ar(Node_ar& root,Node_ar& curr) 
{ 
    if (root.word == L"" ) 
    { 
        // if it is the first Node 
        // then make it the root Node 
		// tree[0] = curr; //??
        root = curr; 
        return; 
    } 
  
    // else: get its editDist from the Root Node 
    int dist = editDistance_ar(curr.word,root.word); 
  
    if (tree_ar[root.next[dist]].word == L"") 
    { 
        /* if no Node exists at this dist from root 
         * make it child of root Node*/
  
        // incrementing the pointer for curr Node 
        ptr_ar++; // should this move down after indexing? --> No!, at ptr=0 there is the root Node. 
  
        // adding curr Node to the tree 
        tree_ar[ptr_ar] = curr; 
  
        // curr as child of root Node 
        root.next[dist] = ptr_ar; 
    } 
    else
    { 
        // recursively find the parent for curr Node 
        add_ar(tree_ar[root.next[dist]],curr); 
    } 
} 
  
// main functionality (must have built tree before using it)
vector <wstring> BK_Tree_getSimilarWords_ar(Node_ar& root, wstring& s, const int tol)  // vectors are slow. might change later?
{ 
    vector < wstring > ret; 
    if (root.word == L"") 
       return ret; 
  
    // calculating editdistance of s from root 
    int dist = editDistance_ar(root.word,s); 
  
    // if dist is less than tolerance value 
    // add it to similar words 
    if (dist <= tol) ret.push_back(root.word); 
  
    // iterate over the string havinng tolerance 
    // in range (dist-TOL , dist+TOL) 
    int start = dist - tol; 
    if (start < 0) 
       start = 1; 
  
    while (start <= dist + tol) //<= not just < ? YES ! IT WAS A MISTAKE !
	// stop if the least closest child to the root has edit distance > dist+TOL.. there is nothing useful under this node
    { 
        vector <wstring> tmp = 
             BK_Tree_getSimilarWords_ar(tree_ar[root.next[start]],s, tol); 
        for (auto i : tmp) 
            ret.push_back(i); 
        start++; 
    } 
    return ret; 
} 


/* SymSpell Utilities */

void print_umap_to_file(string& file_path, unordered_map<wstring, unordered_set<wstring>>& umap){

	wofstream output_file(file_path);

	int temp_cntr;
	unordered_map<wstring, unordered_set<wstring>>:: iterator p;

	for (p = umap.begin(); p != umap.end(); p++){
        output_file <<  p->first << "::"<< endl; // key
		temp_cntr = 0;
		for (wstring x: p->second){ temp_cntr++; output_file << temp_cntr << ":" << x << endl;} // print list of values
		output_file << L"-------------------------" << endl;

		}

		output_file.close();
	}

unordered_map<wstring, unordered_set<wstring>> create_symspell_umap_from_dict_en(vector<wstring>& en_dict, int edit_distance){

	unordered_map<wstring, unordered_set<wstring>> returned_umap;

	wstring w1, w2;
	wstring temp_wstring;

	if (edit_distance ==0){
		for(wstring dict_word: en_dict){
			temp_wstring = dict_word;
			returned_umap[temp_wstring].insert(dict_word);
		}
	}

	if (edit_distance ==1){
		
		// first, create edit_distance = 0 map;
		returned_umap = create_symspell_umap_from_dict_en(en_dict, 0);

		for(wstring dict_word: en_dict){
			if(dict_word.length() == 0){continue;} //skip empty words
			if(dict_word.length() == 1) {returned_umap[L""].insert(dict_word); continue;} // if 1-letter word, just append it to empty string

			for(int i =0; i< dict_word.length(); i++){
				temp_wstring = dict_word;
				temp_wstring.erase(i,1);
				returned_umap[temp_wstring].insert(dict_word);
				}
			}	
	
		}

	
	if (edit_distance ==2){

		// first, create edit_distance = 1 map;
		returned_umap = create_symspell_umap_from_dict_en(en_dict, 1);

		// now, create edit_distance  = 2 map and append it;
		for(wstring dict_word: en_dict){
			if(dict_word.length() == 0 || dict_word.length() == 1){continue;} //skip empty words. words with length=1 were dealt with earlier.
			if(dict_word.length() == 2) {returned_umap[L""].insert(dict_word); continue;} // if 2-letter word, just append it to empty string

			for(int i =0; i< dict_word.length(); i++){
				for (int j =i; j < dict_word.length(); j++){
					if ( i == j){continue;} // since this is equivalent to deleting one word only
					if ( i > j ){ // delete i then delete j (delete larger first so that the smaller index is unchanged in the erase
						temp_wstring = dict_word;
						temp_wstring.erase(i,1);
						temp_wstring.erase(j,1);
					}
					if( i < j){ /// delete j then delete i (delete larger first so that the smaller index is unchanged in the erase
						temp_wstring = dict_word;
						temp_wstring.erase(j,1);
						temp_wstring.erase(i,1);
					}

					returned_umap[temp_wstring].insert(dict_word);

				}
					
				

				
			}
		}	
	
	}



	if (edit_distance ==3){

		// first, create edit_distance = 2 map;
		returned_umap = create_symspell_umap_from_dict_en(en_dict, 2);


		for(wstring dict_word: en_dict){
			if(dict_word.length() == 0 || dict_word.length() == 1 || dict_word.length() == 2){continue;} //skip empty words. words with length=1 and length=2 were dealt with earlier.
			if(dict_word.length() == 3) {returned_umap[L""].insert(dict_word); continue;} // if 2-letter word, just append it to empty string

			temp_wstring = dict_word; // if we did not exit by earlier continue's, then we need this

			for(int i =0; i< dict_word.length(); i++){
				for (int j =i; j < dict_word.length(); j++){
					for(int k=j; k <dict_word.length(); k++){
						temp_wstring = dict_word; // if we did not exit by earlier continue's, then we need this

						if(k==i || k==j || i==j){continue;} // we don't want this, this is equivalent to the distance = 2 or 1 (if all equal) case

						if(i== max(i,j,k)){ // i> j and i > k
							temp_wstring.erase(i,1); // delete at larger first. largest is i
							if(j>k){temp_wstring.erase(j,1); temp_wstring.erase(k,1);} // delete at index j first
							else if(k>j){temp_wstring.erase(k,1); temp_wstring.erase(j,1);} // delete at index k first
						}
						else if(j == max(i, j, k)){ // j>i , j>k
							temp_wstring.erase(j,1); // delete at larger first. largest is j
							if(i>k){temp_wstring.erase(i,1); temp_wstring.erase(k,1);} // delete at index i first
							else if(k>i){temp_wstring.erase(k,1); temp_wstring.erase(i,1);} // delete at index k first
						}
						else if(k == max(i, j, k)){ // k>i, k>j
							temp_wstring.erase(k,1); // delete at larger first. largest is k
							if(i>j){temp_wstring.erase(i,1); temp_wstring.erase(j,1);} // delete at index i first
							else if(j>i){temp_wstring.erase(j,1); temp_wstring.erase(i,1);} // delete at index j first
						}
					

						returned_umap[temp_wstring].insert(dict_word);
					}
				}
			}



		}

	}
	

	return returned_umap;
}

unordered_map<wstring, unordered_set<wstring>> create_symspell_umap_from_dict_ar(vector<wstring>& ar_dict, int edit_distance){

	unordered_map<wstring, unordered_set<wstring>> returned_umap;

	wstring w1, w2;
	wstring temp_wstring;

	if (edit_distance ==0){
		for(wstring dict_word: ar_dict){
			temp_wstring = dict_word;
			returned_umap[temp_wstring].insert(dict_word);
		}
	}

	if (edit_distance ==1){
		
		// first, create edit_distance = 0 map;
		returned_umap = create_symspell_umap_from_dict_ar(ar_dict, 0);

		for(wstring dict_word: ar_dict){
			if(dict_word.length() == 0){continue;} //skip empty words
			if(dict_word.length()/2 == 1) {returned_umap[L""].insert(dict_word); continue;} // if 1-word letter just append it to empty string

			for(int i =0; i< dict_word.length()/2; i++){ // notice we divided by two ! unicode char = 16 bits = 2* byte = 2 *char
				// w1 = dict_word.substr(0,i*2);
				// w2 = dict_word.substr((i+1)*2, (dict_word.length()-i-1)*2);
				temp_wstring = dict_word;
				temp_wstring.erase(2*i,2); // delete 2 bytes = 1 arabic char, starting from i*2
				returned_umap[temp_wstring].insert(dict_word);
				
				}
			}	
	
		}


	if (edit_distance ==2){

		// first, create edit_distance = 1 map;
		returned_umap = create_symspell_umap_from_dict_ar(ar_dict, 1);

		// now, create edit_distance  = 2 map and append it;
		for(wstring dict_word: ar_dict){
			if(dict_word.length() == 0 || dict_word.length()/2 == 1){continue;} //skip empty words. words with length=1 were dealt with earlier.
			if(dict_word.length()/2 == 2) {returned_umap[L""].insert(dict_word); continue;} // if 2-letter word, just append it to empty string

			for(int i =0; i< dict_word.length()/2; i++){
				for (int j =i; j < dict_word.length()/2; j++){
					if ( i == j){continue;} // since this is equivalent to deleting one letter only
					if ( i > j ){ // delete i then delete j (delete larger first so that the smaller index is unchanged in the erase
						temp_wstring = dict_word;

						temp_wstring.erase(i*2,2);
						// temp_wstring.erase(i*2,1);
						temp_wstring.erase(j*2,2);
						// temp_wstring.erase(j*2,1);


					}
					if( i < j){ /// delete j then delete i (delete larger first so that the smaller index is unchanged in the erase
						temp_wstring = dict_word;

						temp_wstring.erase(j*2,2);
						// temp_wstring.erase(i*2,1);
						temp_wstring.erase(i*2,2);
						// temp_wstring.erase(j*2,1);
	
					}

					returned_umap[temp_wstring].insert(dict_word);

				}
				
				
			}
		}	
	
	}


	if (edit_distance ==3){

		// first, create edit_distance = 2 map;
		returned_umap = create_symspell_umap_from_dict_ar(ar_dict, 2);


		for(wstring dict_word: ar_dict){
			if(dict_word.length()/2 == 0 || dict_word.length()/2 == 1 || dict_word.length()/2 == 2){continue;} //skip empty words. words with length=1 and length=2 were dealt with earlier.
			if(dict_word.length()/2 == 3) {returned_umap[L""].insert(dict_word); continue;} // if 2-letter word, just append it to empty string

			temp_wstring = dict_word; // if we did not exit by earlier continue's, then we need this

			for(int i =0; i< dict_word.length()/2; i++){
				for (int j =i; j < dict_word.length()/2; j++){
					for(int k=j; k <dict_word.length()/2; k++){
						temp_wstring = dict_word; // reset deletes every iteration

						if(k==i || k==j || i==j){continue;} // we don't want this, this is equivalent to the distance = 2 or 1 (if all equal) case

						if(i== max(i,j,k)){ // i> j and i > k
							temp_wstring.erase(2*i,2); // delete at larger first. largest is i
							if(j>k){temp_wstring.erase(2*j,2); temp_wstring.erase(2*k,2);} // delete at index j first
							else if(k>j){temp_wstring.erase(2*k,2); temp_wstring.erase(2*j,2);} // delete at index k first
						}
						else if(j == max(i, j, k)){ // j>i , j>k
							temp_wstring.erase(2*j,2); // delete at larger first. largest is j
							if(i>k){temp_wstring.erase(2*i,2); temp_wstring.erase(2*k,2);} // delete at index i first
							else if(k>i){temp_wstring.erase(2*k,2); temp_wstring.erase(2*i,2);} // delete at index k first
						}
						else if(k == max(i, j, k)){ // k>i, k>j
							temp_wstring.erase(2*k,2); // delete at larger first. largest is k
							if(i>j){temp_wstring.erase(2*i,2); temp_wstring.erase(2*j,2);} // delete at index i first
							else if(j>i){temp_wstring.erase(2*j,2); temp_wstring.erase(2*i,2);} // delete at index j first
						}
					

						returned_umap[temp_wstring].insert(dict_word);

					}
				}
			}



		}

	}



	return returned_umap;
}

void union_unordered_sets(unordered_set<wstring>& unioned_in, unordered_set<wstring>& to_union){
	// produces the union of two unordered_sets in the first set
	for ( wstring x : to_union){unioned_in.insert(x);}
}

unordered_set <wstring> get_word_edits_en(wstring& s, const int tol){

	unordered_set <wstring> returned_set, temp_set;
	wstring temp_wstring;

	if(tol==0){ returned_set.insert(s); return returned_set;}

	if(tol==1)
	{
		if(s.length() == 0 || s.length() == 1) {returned_set.insert(L""); return returned_set;} // if 1-letter word, just append it to empty string

		// generate less tol's possibilities first:
		returned_set = get_word_edits_en(s, 0);

		// generate possibilities for tol = 1
		for(int i =0; i< s.length(); i++) // only if s.length() >= 2
		{
				temp_wstring = s;
				temp_wstring.erase(i,1);
				returned_set.insert(temp_wstring);
		}
	}


	if (tol ==2){
		if(s.length() == 0) {returned_set.insert(L""); return returned_set;} // if 1-letter word, just append it to empty string
		if(s.length() == 1) {return get_word_edits_en(s,1);} // if 1-letter word, just append it to empty string

		// generate less tol's possibilities first:
		returned_set = get_word_edits_en(s,1); // generates for tol = 0, too, inside it

		for(int i =0; i< s.length(); i++){
				for (int j =i+1; j < s.length(); j++){ // j is always > i. so, delete at j first.
					temp_wstring = s;
					temp_wstring.erase(j,1);
					temp_wstring.erase(i,1);
					returned_set.insert(temp_wstring);
				}
		}
	}



	if (tol ==3){
		if(s.length() == 0) {returned_set.insert(L""); return returned_set;} // if 1-letter word, just append it to empty string
		if(s.length() == 1) {return get_word_edits_en(s,1);} // if 1-letter word, just append it to empty string
		if(s.length() == 2) {return get_word_edits_en(s,2);} // if 1-letter word, just append it to empty string


		// generate less tol's possibilities first:
		returned_set = get_word_edits_en(s,2); // generates for tol = 0 and tol =1, too, inside it

		for(int i =0; i< s.length(); i++){
				for (int j =i+1; j < s.length(); j++){ // j is always > i. so, delete at j first.
					for (int k = j+1; k < s.length(); k++){
					temp_wstring = s;
					temp_wstring.erase(k,1);
					temp_wstring.erase(j,1);
					temp_wstring.erase(i,1);
					returned_set.insert(temp_wstring);

					}
				}
		}
	}
	
	return returned_set;

}

unordered_set <wstring> SymSpell_getSimilarWords_en(unordered_map<wstring, unordered_set<wstring>>& umap_en_dict, wstring& s,
	const int tol)
{
	unordered_set< wstring > possible_edits_for_word; 
	unordered_set< wstring > ret;

	possible_edits_for_word = get_word_edits_en(s, tol);

	// IMPORTANT: assumes generated umap_en_dict was generated with equivalent edit distance

	for (wstring edited_word : possible_edits_for_word)
	{
		for (wstring x: umap_en_dict[edited_word])
		{
			ret.insert(x);
		}
	}
	


	return ret;
}

unordered_set <wstring> get_word_edits_ar(wstring& s, const int tol){

	unordered_set <wstring> returned_set, temp_set;
	wstring temp_wstring;

	if(tol==0){ returned_set.insert(s); return returned_set;}

	if(tol==1)
	{
		if(s.length()/2 == 0 || s.length()/2 == 1) {returned_set.insert(L""); return returned_set;} // if 1-letter word, just append it to empty string

		// generate less tol's possibilities first:
		returned_set = get_word_edits_ar(s, 0);

		// generate possibilities for tol = 1
		for(int i =0; i< s.length()/2; i++) // only if s.length() >= 2
		{
				temp_wstring = s;
				temp_wstring.erase((2*i),2);
				returned_set.insert(temp_wstring);
		}
	}


	if (tol ==2){
		if(s.length()/2 == 0) {returned_set.insert(L""); return returned_set;} // if 1-letter word, just append it to empty string
		if(s.length()/2 == 1) {return get_word_edits_ar(s,1);} // if 1-letter word, just append it to empty string

		// generate less tol's possibilities first:
		returned_set = get_word_edits_ar(s,1); // generates for tol = 0, too, inside it

		for(int i =0; i< s.length()/2; i++){
				for (int j =i+1; j < s.length()/2; j++){ // j is always > i. so, delete at j first.
					temp_wstring = s;
					temp_wstring.erase(2*j,2);
					temp_wstring.erase(2*i,2);
					returned_set.insert(temp_wstring);
				}
		}
	}



	if (tol ==3){
		if(s.length()/2 == 0) {returned_set.insert(L""); return returned_set;} // if 1-letter word, just append it to empty string
		if(s.length()/2 == 1) {return get_word_edits_en(s,1);} // if 1-letter word, just append it to empty string
		if(s.length()/2 == 2) {return get_word_edits_en(s,2);} // if 1-letter word, just append it to empty string


		// generate less tol's possibilities first:
		returned_set = get_word_edits_ar(s,2); // generates for tol = 0 and tol =1, too, inside it

		for(int i =0; i< s.length()/2; i++){
				for (int j =i+1; j < s.length()/2; j++){ // j is always > i. so, delete at j first.
					for (int k = j+1; k < s.length()/2; k++){
					temp_wstring = s;
					temp_wstring.erase(2*k,2);
					temp_wstring.erase(2*j,2);
					temp_wstring.erase(2*i,2);
					returned_set.insert(temp_wstring);

					}
				}
		}
	}
	
	return returned_set;

}

unordered_set <wstring> SymSpell_getSimilarWords_ar(unordered_map<wstring, unordered_set<wstring>>& umap_ar_dict, wstring& s,
	const int tol)
{
	unordered_set< wstring > possible_edits_for_word; 
	unordered_set< wstring > ret;

	possible_edits_for_word = get_word_edits_ar(s, tol);

	// IMPORTANT: assumes generated umap_ar_dict was generated with equivalent edit distance

	for (wstring edited_word : possible_edits_for_word)
	{
		for (wstring x: umap_ar_dict[edited_word])
		{
			ret.insert(x);
		}
	}
	


	return ret;
}



// function to find words: hash to same place, then set from list, then print to file, and we are done



// driver program to run above functions 
int main(int argc, char const *argv[]) 
{ 

	/* vars */
	string dictionary_path = "C:/Users/DELL/Desktop/Words_unique.txt";	// original dictionary path
	string infile_path = "C:/Users/DELL/Desktop/infile.txt";			// file with (one) word to be corrected (r)
	string list_ar_path = "C:/Users/DELL/Desktop/Words_Ar.txt";			// file to save arabic dictionary in (rw)
	string list_en_path = "C:/Users/DELL/Desktop/Words_En.txt";			// file to save english dictionary in (rw)

	string linspell_guess_path = "C:/Users/DELL/Desktop/linspell_guess.txt";	// file to save corrections guessed by LinSpell
	string bk_tree_guess_path = "C:/Users/DELL/Desktop/bk_tree_guess.txt";		// file to save corrections guessed by BK-Tree algorithm
	string umap_file_path_en = "C:/Users/DELL/Desktop/umap_printout_en.txt";	// file to print out the english unordered map
	string umap_file_path_ar = "C:/Users/DELL/Desktop/umap_printout_ar.txt";	// file to print out the arabic unordered map
	string symspell_guess_path = "C:/Users/DELL/Desktop/symspell_guess.txt";	// file to save corrections guessed by BK-Tree algorithm

	// Timing Vars
	auto start = std::chrono::high_resolution_clock::now();
	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed;

    /* Input Word */
	wstring input_word; // to store input word
	bool read = false; // used to ensure only first word from the input file is considered

	std::wifstream input_word_file(infile_path);

	while (std::getline(input_word_file, input_word) && !read) {// read first word of input file
		if(input_word.length() >0) { 
			read=true;
		}
	}

	input_word_file.close();


    /* Input Dictionary */

	// Separate English and Arabic Dictionaries from souce dictionary into to files:
	std::wifstream words(dictionary_path);

	bool print_num_words_each = true;
	separate_dicts(words, list_ar_path,  list_en_path, print_num_words_each);
	words.close();


	// Read both (EN and AR) dictionaries into two lists:
	vector < wstring > ar_dict;  vector < wstring > en_dict;

	std::wifstream ar_dict_file(list_ar_path); 
	std::wifstream en_dict_file(list_en_path);

	wstring wline; 

	while (std::getline(ar_dict_file, wline)) {// read all arabic words into vector
		ar_dict.push_back(wline);
	}

	while (std::getline(en_dict_file, wline)) {// read all arabic words into vector
		en_dict.push_back(wline);
	}

	ar_dict_file.close();
	en_dict_file.close();



	/* LinSpell */

	
	std::wofstream linspell_guess(linspell_guess_path); // file to store linspell guesses

	vector <wstring> linspell_words = LinSpell_getSimilarWords_ar(input_word, ar_dict, TOL);
	for (wstring x : linspell_words) 
         linspell_guess << x <<  endl; 

	linspell_guess.close();

	// Timing
	start = std::chrono::high_resolution_clock::now();
	for (int i=0; i<NUM_RUNS; i++){
		linspell_words = LinSpell_getSimilarWords_ar(input_word, ar_dict, TOL);
	}
	finish = std::chrono::high_resolution_clock::now();

	elapsed = finish - start;
	cout << "Elapsed time for "<< NUM_RUNS <<" runs of LinSpell is: "<<  elapsed.count() << " s\n";
	

	/* BK-Tree */
	// Build Tree

	
	start = std::chrono::high_resolution_clock::now();
	for (wstring word_to_add: ar_dict){
			Node_ar tmp = Node_ar(word_to_add); 
			add_ar(RT_ar,tmp); 
	}
	finish = std::chrono::high_resolution_clock::now();
	elapsed = finish - start;
	cout << "Elapsed time to build one BK-Tree is: " << elapsed.count() << " s\n";


	std::wofstream bk_tree_guess(bk_tree_guess_path); // file to store bk_tree guesses
	vector < wstring > bk_tree_words = BK_Tree_getSimilarWords_ar(RT_ar, input_word, TOL); 
	for (wstring x : bk_tree_words) 
         bk_tree_guess << x <<  endl; 
	bk_tree_guess.close();

	// Timing
	start = std::chrono::high_resolution_clock::now();
	for (int i=0; i<NUM_RUNS; i++){
		linspell_words = BK_Tree_getSimilarWords_ar(RT_ar, input_word, TOL);
	}
	finish = std::chrono::high_resolution_clock::now();

	elapsed = finish - start;
	cout << "Elapsed time for "<< NUM_RUNS <<" runs of BK_Tree without recreating the tree is: "<<  elapsed.count() << " s\n";
	



	/* SymSpell */

	unordered_map<wstring, unordered_set<wstring>> umap_en_dict, umap_ar_dict; 
	
	// umap_en_dict = create_symspell_umap_from_dict_en(en_dict, TOL);
	//print_umap_to_file(umap_file_path_en, umap_en_dict);



	// Timing
	start = std::chrono::high_resolution_clock::now();
	umap_ar_dict = create_symspell_umap_from_dict_ar(ar_dict, TOL);
	finish = std::chrono::high_resolution_clock::now();

	elapsed = finish - start;
	cout << "Elapsed time to create the SymSpell hash map:  " << elapsed.count() << " s\n";

	
	// Print it to file for visualization (time not taken into account since not part of the algorithm):
	print_umap_to_file(umap_file_path_ar, umap_ar_dict);

	// Retrieve guessed words
	unordered_set <wstring> symspell_words =  SymSpell_getSimilarWords_ar( umap_ar_dict, input_word, TOL);	

	// Print guessed words to file
	std::wofstream symspell_guess(symspell_guess_path); // file to store bk_tree guesses
	for (wstring x : symspell_words) 
         symspell_guess << x <<  endl; 
	symspell_guess.close();


	// Timing
	start = std::chrono::high_resolution_clock::now();
	for (int i=0; i<NUM_RUNS; i++){
		symspell_words =  SymSpell_getSimilarWords_ar( umap_ar_dict, input_word, TOL);
	}
	finish = std::chrono::high_resolution_clock::now();

	elapsed = finish - start;
	cout << "Elapsed time for "<< NUM_RUNS <<" runs of SymSpell without hash map recreation time is: "<<  elapsed.count() << " s\n";


	std::getchar();
    return 0; 
}



//string s( ws.begin(), ws.end() );
