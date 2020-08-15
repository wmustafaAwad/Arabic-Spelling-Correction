
#include "proj_utils.h"

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
