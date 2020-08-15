#pragma once
#include "proj_utils.h"


/**
Implements the BK-Tree Spelling Check.

	Using the edit_distance from proj_utils. Organizes the dictionary into a tree with known edit_distances.
Utilizes the fact that levneshtein distances defines a metric-space to speed-up later search using an input word.

**/

/** This file defines the BK_Tree class and the Node_ar struct **/

struct Node_ar // ar: for arabic. 
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

class BKTree{

private:
	void add_ar(Node_ar& root,Node_ar& curr);
	// stores the root Node of the arabic words bk-tree 
  
	// stores every Node of the arabic words bk-tree 
	Node_ar tree_ar[MAXN]; 
  
	// index for current Node of the arabic words bk-tree 

	

public:
	Node_ar RT_ar;
	vector <wstring> my_ar_dict;
	int ptr_ar; // set to zero? --> Actually done in main.

	vector <wstring> BK_Tree_getSimilarWords_ar(Node_ar& root, wstring& s, const int tol);
	BKTree();
	void build_tree();

};



