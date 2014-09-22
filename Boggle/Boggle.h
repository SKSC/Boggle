//
//  Boggle.h
//  Boggle
//
//  Created by Chen Sha on 22/09/2014.
//  Copyright (c) 2014 Chen Sha. All rights reserved.
//

#ifndef __Boggle__Boggle__
#define __Boggle__Boggle__

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include "math.h"
#include<time.h>
#include <unordered_map>;

// internally stores a dictionary of words and can then return a list of
// all of the dictionary's words found in a given boggle grid
class Boggle
{
    
public:
    // takes the given filepath and loads a dictionary file from it
    Boggle( const char * a_DictionaryFile );
    
    // returns the number of words in the dictionary
    unsigned int GetSize() const;
    
    // finds all words in the given grid and puts them in the results vector
    void Solve( const char * a_Grid, std::vector< std::string > & a_Results ) const;
    
    void simpleSolver( const char * a_Grid, std::vector< std::string > & a_Results ) const;
    void complexSolver( const char * a_Grid, std::vector< std::string > & a_Results)  const;
    //
    void wordSearch(std::string word, int row, int col, int size, bool** grid, char** boggleTable, std::vector< std::string > & a_Results)const;
    bool gridChecker(std::string word, int currentIndex, int row, int col, bool** grid, char** boggleTable, int  wordLength,std::vector< std::string > & a_Results )const;
    ~Boggle();
};

#endif /* defined(__Boggle__Boggle__) */
