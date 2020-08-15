#include "proj_utils.h"
#include "LinSpell.h"
#include "BKTree.h"
#include "SymSpell.h"


BKTree my_bk_tree;
SymSpellUtil my_symspell_util;



// function to find words: hash to same place, then set from list, then print to file, and we are done



// driver program to run above functions 
int main(int argc, char const *argv[]) 
{ 

	/* vars */
	string main_folder = "C:/Users/DELL/Desktop/";

	string dictionary_path = main_folder+"Words_unique.txt";		// original dictionary path
	string infile_path = main_folder+"infile.txt";					// file with (one) word to be corrected (r)
	string list_ar_path = main_folder+"Words_Ar.txt";				// file to save arabic dictionary in (rw)
	string list_en_path = main_folder+"Words_En.txt";				// file to save english dictionary in (rw)

	string linspell_guess_path = main_folder+"linspell_guess.txt";	// file to save corrections guessed by LinSpell
	string bk_tree_guess_path = main_folder+"bk_tree_guess.txt";	// file to save corrections guessed by BK-Tree algorithm
	string umap_file_path_en = main_folder+"umap_printout_en.txt";	// file to print out the english unordered map
	string umap_file_path_ar = main_folder+"umap_printout_ar.txt";	// file to print out the arabic unordered map
	string symspell_guess_path = main_folder+"symspell_guess.txt";	// file to save corrections guessed by BK-Tree algorithm

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

	// Initialize :	 (faced some problems with non-default constructors)
	my_bk_tree.my_ar_dict = ar_dict;
	my_bk_tree.ptr_ar = 0;

	// Build Tree
	start = std::chrono::high_resolution_clock::now();
	my_bk_tree.build_tree();
	finish = std::chrono::high_resolution_clock::now();

	elapsed = finish - start;
	cout << "Elapsed time to build one BK-Tree is: " << elapsed.count() << " s\n";

	// Get Similar Words
	vector < wstring > bk_tree_words = my_bk_tree.BK_Tree_getSimilarWords_ar(my_bk_tree.RT_ar, input_word, TOL); 

	// Print them to file
	std::wofstream bk_tree_guess(bk_tree_guess_path); // file to store bk_tree guesses
	for (wstring x : bk_tree_words) 
         bk_tree_guess << x <<  endl; 
	bk_tree_guess.close();

	// Timing
	start = std::chrono::high_resolution_clock::now();
	for (int i=0; i<NUM_RUNS; i++){
		bk_tree_words = my_bk_tree.BK_Tree_getSimilarWords_ar(my_bk_tree.RT_ar, input_word, TOL);
	}
	finish = std::chrono::high_resolution_clock::now();

	elapsed = finish - start;
	cout << "Elapsed time for "<< NUM_RUNS <<" runs of BK_Tree without recreating the tree is: "<<  elapsed.count() << " s\n";






	/* SymSpell */

	// Build Hashmap:
	start = std::chrono::high_resolution_clock::now();
	my_symspell_util.create_symspell_umap_from_dict_ar(ar_dict, TOL);
	finish = std::chrono::high_resolution_clock::now();

	elapsed = finish - start;
	cout << "Elapsed time to create the SymSpell hash map:  " << elapsed.count() << " s\n";

	
	// Print it to file for visualization (time not taken into account since not part of the algorithm):
	print_umap_to_file(umap_file_path_ar, my_symspell_util.my_umap_ar_dict);

	
	// Retrieve guessed words
	unordered_set <wstring> symspell_words =  my_symspell_util.SymSpell_getSimilarWords_ar(input_word, TOL);	

	// Print guessed words to file
	std::wofstream symspell_guess(symspell_guess_path); // file to store symspell guesses
	for (wstring x : symspell_words) 
         symspell_guess << x <<  endl; 
	symspell_guess.close();


	// Timing
	start = std::chrono::high_resolution_clock::now();
	for (int i=0; i<NUM_RUNS; i++){
		symspell_words =  my_symspell_util.SymSpell_getSimilarWords_ar(input_word, TOL);
	}
	finish = std::chrono::high_resolution_clock::now();

	elapsed = finish - start;
	cout << "Elapsed time for "<< NUM_RUNS <<" runs of SymSpell without hash map recreation time is: "<<  elapsed.count() << " s\n";



	

	std::getchar();
    return 0; 
}



//string s( ws.begin(), ws.end() );
