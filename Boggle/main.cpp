//
//  main.cpp
//  Boggle
//
//  Created by Chen Sha on 22/09/2014.
//  Copyright (c) 2014 Chen Sha. All rights reserved.
//

#include <iostream>
#include "Boggle.h"
#include <vector>

static Boggle s_Boggle("dict.txt");

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    std::vector<std::string> results;
    s_Boggle.Solve("pesa", results);
    s_Boggle.Solve("meusdeloa", results);
    
    return 0;
}
