#pragma once
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
#define LEN 20  // Changed from 10 to 20 since some words had difference > 10 (eg: أعطيتمونيهم)

# define NUM_RUNS 100 
// Number of runs to average over for benchmark purposes


/* Language Utilities */
bool is_english(wstring& a);
void separate_dicts(wifstream& mixed_words_wifstream, string& output_path_ar, string& output_path_en, bool print_num_words_each);


/* Distance Utilities */
int min(int a, int b, int c);
int max (int a, int b, int c);
int editDistance_ar(wstring& a,wstring& b);
int editDistance_en(string& a,string& b);


/* SymSpell Utilities */
void print_umap_to_file(string& file_path, unordered_map<wstring, unordered_set<wstring>>& umap);
