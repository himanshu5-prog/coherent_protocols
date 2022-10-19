#include<iostream>
//#include "test/mem_test.hpp"
#include "test/sys_test.hpp"
#include <cstring>
using namespace std;

int main(int argc, char** argv){
    //string testName = "SingleRead";
    string fileName = "input/simple_read.txt";
    //memTest();
    cout << "argument count: " << argc << "\n";
    assert (argc >= 2)    ;

    //testName = argv[1];
    fileName = argv[1];

    bool verbose = true;
    
    cout << " argv[2]: " << argv[2] << "\n";
    if (strcmp(argv[2], "-v" ) == 0){
        assert (argc == 4);
        cout << "argv[3]: " << argv[3] << "\n";
        //assert ( argv[3] == "1" || argv[3] == "0" );
        cout << "Found -v switch\n"; 
        if ( !(strcmp(argv[3], "0")) ){
            verbose = false;
        } else if ( !(strcmp(argv[3], "1")) ){
            verbose = true;
        }
    }
    
    std :: cout << "Running test: " << fileName << ", verbosity: " << verbose<< "\n";
    sys_test(fileName, verbose);
    
    
    return 0;
}