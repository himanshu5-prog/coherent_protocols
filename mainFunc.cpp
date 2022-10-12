#include<iostream>
//#include "test/mem_test.hpp"
#include "test/sys_test.hpp"
using namespace std;

int main(int argc, char** argv){
    //string testName = "SingleRead";
    string fileName = "input/simple_read.txt";
    //memTest();

    assert (argc >= 2)    ;

    //testName = argv[1];
    fileName = argv[1];

    bool verbose = false;

    if (argv[2] == "-v"){
        assert (argc == 4);
        assert ( argv[3] == "1" || argv[3] == "0" );
        
        if (argv[3] == "0"){
            verbose = false;
        } else if ( argv[3] == "1"){
            verbose = true;
        }
    }

    std :: cout << "Running test: " << fileName << "\n";
    sys_test(fileName, verbose);
    
    
    return 0;
}