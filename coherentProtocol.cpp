#include<iostream>
//#include "test/mem_test.hpp"
#include "test/sys_test.hpp"
#include <cstring>
#include <fstream>
#include <sstream>
#include <cassert>
#include <unordered_map>
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
//------------------------------------------------------------
    std :: unordered_map <Parameters, Params_t> parameters;
    //Setting default values
    /*
      CORE_TO_BUS_BUF_SIZE: 16
      CORE_TO_BUS_RESP_BUF_SIZE: 16
      BUS_TO_CORE_BUF_SIZE: 16
      BUS_TO_MEM_BUF_SIZE: 16
      MEM_TO_BUS_BUF_SIZE: 16  
    */
    parameters[Parameters::CORE_TO_BUS_BUF_SIZE] = 16; // Core parameter
    parameters[Parameters::CORE_TO_BUS_RESP_BUF_SIZE] = 16; // Core parameter
    parameters[Parameters::BUS_TO_CORE_BUF_SIZE] = 16; // Bus parameter
    parameters[Parameters::BUS_TO_MEM_BUF_SIZE] = 16; // Bus parameter
    parameters[Parameters::MEM_TO_BUS_BUF_SIZE] = 16; // Memory parameter

    // Read parameters from a text file
    ifstream file("param.txt"); // Open the text file
    bool fileFound;
    if (!file) {
        cerr << "Error: Could not open file.\n";
        fileFound = false;
    } else {
        fileFound = true;
    }

    if (fileFound == true){
        string line;
        while (getline(file, line)) { // Read file line by line
            istringstream iss(line);
            string key;
            int value;

            if (getline(iss, key, ':') && iss >> value) { // Extract key and value
                key.erase(key.find_last_not_of(" \t") + 1); // Trim trailing spaces
                parameters[convertStringToParameter(key)] = value;
            }
        }
        file.close(); // Close the file
    }
    //-------------------------------------------
    
    // Now we can use the parameters in the test
    // Now let's run the test
    std :: cout << "Running test: " << fileName << ", verbosity: " << verbose<< "\n";
    sys_test(fileName, verbose, parameters);
    
    
    return 0;
}