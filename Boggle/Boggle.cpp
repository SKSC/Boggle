//
//  Boggle.cpp
//  Boggle
//
//  Created by Chen Sha on 22/09/2014.
//  Copyright (c) 2014 Chen Sha. All rights reserved.
//

#include "Boggle.h"


using namespace std;

//
const int threshold = 3;
int gridSize=0;

//use it when grid size is smaller than threshold*threshold
//the algorithm is recursion
unordered_map<string, int> dict_small_grid;

//use these when grid size is larger than threshold * threshold
//the algorithm is taking a word from dictionary and checking whether this word can be found in the grid
vector<string> dict_big_grid;
//each point has 8 adjoint points
int dRow[] = {0, 1, 1, 1, 0, -1, -1, -1};
int dCol[] = {1, 1, 0, -1, -1, -1, 0, 1};

Boggle::Boggle( const char * a_DictionaryFile )
{
    //At this stage, not able to determine the lenght of a_Grid
    //read dictionary from file, put word into both containers
    //double the loading time
    ifstream infile;
    infile.open(a_DictionaryFile);
    string str;
    while(getline(infile,str))
    {
        dict_small_grid[str]=1;
        dict_big_grid.push_back(str);
    }
    infile.clear();
    infile.close();
}

unsigned int Boggle::GetSize() const
{
    return dict_small_grid.size();
}

void Boggle::Solve( const char * a_Grid, std::vector< std::string > & a_Results ) const
{
    //assume the letter coordinate is     P E and  M M G
    //                                    S A      E U S
    //                                             K D E
    string str=a_Grid;
    gridSize =  sqrt((double)str.size());
    //if grid is small, use simple solver
    if(gridSize<=threshold){
        simpleSolver(a_Grid,a_Results);
    }else
    {
        //for big complex grid
        complexSolver(a_Grid,a_Results);
    }
    //
    sort(a_Results.begin(),a_Results.end());
}

void Boggle::simpleSolver( const char * a_Grid, std::vector< std::string > & a_Results ) const
{
    string str = a_Grid;
    bool **grid;
    char **boggleTable;
    grid = new bool*[gridSize];
    boggleTable = new char*[gridSize];
    
    //initialize a grid, this is the flag for grid availability
    // initialize a boggle table
    for(int i =0;i<gridSize;i++){
        grid[i] = new bool[gridSize];
        boggleTable[i] = new char[gridSize];
        for(int j=0;j<gridSize;j++){
            grid[i][j]=false;
            boggleTable[i][j] =  a_Grid[gridSize*i+j];
        }
    }
    
    for(int i=0; i< gridSize;i++){
        //clear grid flag
        for(int j=0;j<gridSize;j++){
            string word;
            //recursion
            wordSearch(word, i, j, gridSize,grid, boggleTable, a_Results);
        }
    }
    //
    for(int i=0;i<gridSize;i++)
    {
        delete[] grid[i];
        delete boggleTable[i];
    }
    delete[] grid;
    delete[] boggleTable;
}
void Boggle::complexSolver( const char * a_Grid, std::vector< std::string > & a_Results ) const
{
    int dictSize = dict_big_grid.size();
    unordered_map<char,bool> gridLetter;
    //assume the letter coordinate is     P E and  M M G
    //                                    S A      E U S
    //                                             K D E
    
    //assume sqrt(length(a_Grid)) is an integer
    string str = a_Grid;
    
    bool **grid;
    char **boggleTable;
    grid = new bool*[gridSize];
    boggleTable = new char*[gridSize];
    
    //initialize a grid, this is the flag for grid availability
    // initialize a boggle table
    for(int i =0;i<gridSize;i++){
        grid[i] = new bool[gridSize];
        boggleTable[i] = new char[gridSize];
        for(int j=0;j<gridSize;j++){
            grid[i][j]=false;
            boggleTable[i][j] =a_Grid[gridSize*i+j];
            gridLetter[a_Grid[gridSize*i+j]]=true;
        }
    }
    
    for(int i=0;i<dictSize;i++){
        string word=dict_big_grid[i];
        int maxLength = gridSize*gridSize;
        bool letter_flag = true;
        int wordLength = word.size();
        //ensure all letters can be found in the grid
        if( wordLength>2 &&  wordLength <= maxLength){
            //ensure all letters of the word is on the grid
            for(int k=0;k<wordLength;k++){
                if(gridLetter[word[k]]!=true){
                    letter_flag = false;
                    break;
                }
            }
            if(letter_flag)
            {
                for(int j=0;j<gridSize;j++)
                {
                    for(int k=0;k<gridSize;k++)
                    {
                        if(boggleTable[j][k]==word[0])
                        {
                            grid[j][k]=true;
                            gridChecker(word,0,j,k,grid,boggleTable, wordLength,a_Results);
                            grid[j][k]=false;
                        }
                    }
                }
            }
        }
    }
    for(int i=0;i<gridSize;i++)
    {
        delete[] grid[i];
        delete boggleTable[i];
    }
    delete[] grid;
    delete[] boggleTable;
}

void Boggle::wordSearch(string word, int row, int col, int size, bool** grid, char** boggleTable, std::vector< std::string > & a_Results)const
{
    //Check boundary
    if(row < 0 || col < 0 || row >= size || col >= size)
        return;
    if(grid[row][col] == true)
        return;
    
    //
    bool **grid_tmp;
    grid_tmp = new bool*[size];
    for(int i =0;i<size;i++)
    {
        grid_tmp[i] = new bool[size];
        for(int j=0;j<size;j++)
        {
            grid_tmp[i][j]=grid[i][j];
        }
    }
    
    word+=(boggleTable[row][col]);
    grid_tmp[row][col] = true;
    
    
    if (dict_small_grid[word]==1 && word.size()>=3)
    {
        vector<string>::iterator iter;
        //Store the combination only if it has not been found yet
        iter= find(a_Results.begin(),a_Results.end(),word);
        if(iter==a_Results.end() )
        {
            iter= find(a_Results.begin(),a_Results.end(),word);
            if(iter==a_Results.end() )
            {
                a_Results.push_back(word);
            }
        }
    }
    
    wordSearch( word, row+1, col+1, size, grid_tmp, boggleTable,a_Results);
    wordSearch( word, row-1, col-1, size, grid_tmp, boggleTable, a_Results);
    wordSearch( word, row-1, col, size, grid_tmp, boggleTable,a_Results);
    wordSearch( word, row-1, col+1, size, grid_tmp, boggleTable, a_Results);
    wordSearch( word, row, col-1, size, grid_tmp, boggleTable, a_Results);
    wordSearch( word, row, col+1, size, grid_tmp, boggleTable,a_Results);
    wordSearch( word, row+1, col-1, size, grid_tmp, boggleTable, a_Results);
    wordSearch( word, row+1, col, size, grid_tmp, boggleTable, a_Results);
    wordSearch( word, row+1, col+1, size, grid_tmp, boggleTable, a_Results);
    //
    for(int i=0;i<size;i++)
    {
        delete[] grid_tmp[i];
    }
    delete[] grid_tmp;
}

bool Boggle::gridChecker(std::string word, int currentIndex, int row, int col, bool** grid, char** boggleTable, int  wordLength,std::vector< std::string > & a_Results )const
{
    
    bool **grid_tmp;
    grid_tmp = new bool*[gridSize];
    for(int i =0;i<gridSize;i++)
    {
        grid_tmp[i] = new bool[gridSize];
        for(int j=0;j<gridSize;j++)
        {
            grid_tmp[i][j]=grid[i][j];
        }
    }
    
    bool flag = false;
    if(currentIndex== wordLength-1)
    {
        //if a word is not in the vector, put it in
        vector<string>::iterator iter;
        iter= find(a_Results.begin(),a_Results.end(),word);
        if(iter==a_Results.end() )
        {
            a_Results.push_back(word);
        }
        return true;
    }
    else
    {
        // try 8 adjoint points
        for(int i=0;i<8;i++)
        {
            int newRow = row+dRow[i];
            int newCol = col+dCol[i];
            //
            if(newRow >=0 && newCol>=0 && newRow<gridSize && newCol<gridSize && grid_tmp[newRow][newCol]!=true)
            {
                if(boggleTable[newRow][newCol]==word[currentIndex+1]){
                    currentIndex++;
                    grid_tmp[newRow][newCol]=true;
                    flag = gridChecker(word,currentIndex,newRow,newCol,grid_tmp,boggleTable, wordLength,a_Results);
                    if(flag)
                        break;
                    currentIndex--;
                    grid_tmp[newRow][newCol]=false;
                    
                }
            }
        }
    }
    for(int i=0;i<gridSize;i++)
    {
        delete[] grid_tmp[i];
    }
    delete[] grid_tmp;
    return flag;
}

Boggle::~Boggle()
{
    //release vector dict and map dict
    vector<string> ().swap(dict_big_grid);
    unordered_map<string,int>().swap(dict_small_grid);
}