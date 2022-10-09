#include "cpu.hpp"

void CPU :: printInfo (){
    cout << "Information for CPU\n";
    for(int i=0; i<8; i++){
        core[i].printInfo();
    }
}

void CPU :: set_input_file (string str){
    inputFile = str;
}

void CPU :: push_core_to_bus_q ( core_to_bus_tr reqTr){
    core2bus_q.push (reqTr);
}

void CPU :: push_core_to_bus_resp_q ( core_to_bus_tr reqTr){
    core2bus_resp_q.push (reqTr);
}

void CPU :: push_bus_to_core_q (bus_to_core_tr reqTr){
    bus2core_q.push ( reqTr );
}

void CPU :: pop_core_to_bus_q (){
    core2bus_q.pop();
}

void CPU :: pop_core_to_bus_resp_q (){
    core2bus_resp_q.pop();
}

void CPU :: pop_bus_to_core_q(){
    bus2core_q.pop();
}

void print_vector (vector <string> v){
    cout << "Printing the vector\n";
    for (auto x: v){
        cout << x << " ";
    }
    cout << "\n";
}

void getInst ( string line, vector<string> &v){
    string word = "";
    int count = 0;

    for ( auto x: line){
        if ( x == ' '){
            v.push_back(word);
            count += 1;
            word = "";
        } else {
            word = word + x;
        }
    }
    if (word.length() > 0)
        v.push_back(word);
}
void CPU :: load_inst_q (string fileName){
    ifstream input_file;
    Instruction inst;
    input_file.open(fileName);
    string line;
    vector <string> v;

    if(input_file.is_open()){

        while (getline(input_file, line)){
            //cout << line << endl;
            getInst(line,v);
            //print_vector(v);

            inst.op = v[0];
            inst.address = stoi (v[1]);
            inst.data = stoi(v[2]);
            inst.coreID = stoi(v[3]);
            inst.valid = true;

            inst_q.push (inst);
            
            v.clear();
        }
        
        /*
        cout << "Line: " << line << "\n";
        getInst (line, v);
        cout << " cpu :: load_inst_q : vector size: " << v.size() << "\n";
        print_vector (v);
        */
    }

    input_file.close();
}

void CPU :: run_function (){
    cout << "CPU :: run_function - input file: " << inputFile << "\n";
    load_inst_q (inputFile);
}

int CPU :: get_size_inst_q (){
    return inst_q.size();
}