#include "BKTree.h"


BKTree::BKTree(){}

/**
BKTree::BKTree(vector<wstring> dict){
	my_ar_dict = dict;
	ptr_ar = 0;
}
**/

void BKTree::add_ar(Node_ar& root,Node_ar& curr){
	 if (root.word == L"" ) 
    { 
        // if it is the first Node 
        // then make it the root Node 
		// tree[0] = curr; //??
        root = curr; 
        return; 
    } 
  
    // else: get its editDist from the Root Node 
    int dist = editDistance_ar(curr.word, root.word); 
  
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

void BKTree::build_tree(){
	for (wstring word_to_add: my_ar_dict){
			Node_ar tmp = Node_ar(word_to_add); 
			add_ar(RT_ar,tmp); 
	}
}

// main functionality (must have built tree before using it)
vector <wstring> BKTree::BK_Tree_getSimilarWords_ar(Node_ar& root, wstring& s, const int tol)  // vectors are slow. might change later?
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



/**
class BKTree{

private:
	void add_ar(Node_ar& root,Node_ar& curr);
	// stores the root Node of the arabic words bk-tree 
	 
  
	// stores every Node of the arabic words bk-tree 
	Node_ar tree_ar[MAXN]; 
  
	// index for current Node of the arabic words bk-tree 
	int ptr_ar; // set to zero? --> Actually done in main.

	vector <wstring> my_ar_dict;

public:
	Node_ar RT_ar;
	vector <wstring> BK_Tree_getSimilarWords_ar(Node_ar& root, wstring& s, const int tol);
	BKTree();
	BKTree(vector <wstring>&  dict);
	void build_tree();

};

**/