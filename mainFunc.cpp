#include<iostream>
//#include "test/mem_test.hpp"
#include "test/sys_test.hpp"
using namespace std;

int main(int argc, char** argv){
    //string testName = "SingleRead";
    string fileName = "input/simple_read.txt";
    //memTest();

    assert (argc == 2)    ;

    //testName = argv[1];
    fileName = argv[1];


    std :: cout << "Running test: " << fileName << "\n";
    sys_test(fileName);
    
    
    return 0;
}