#include "cpu.hpp"

void CPU :: printInfo (){
    cout << "Information for CPU\n";
    for(int i=0; i<8; i++){
        m_core_ptr[i]->printInfo();
    }
}

void CPU :: incr_clk_cycle (){
    for (int i=0; i<8; i++){
        m_core_ptr[i]->incr_clk_cycle();
    }
}
/*
void CPU :: set_input_file (string str){
    inputFile = str;
}
*/

void CPU :: set_debug_mode (bool b){
    for (int i=0; i<8; i++){
        m_core_ptr[i]->set_debug_mode(b);
    }
}

void CPU :: push_core_to_bus_q ( core_to_bus_tr reqTr, int id){
    m_core_ptr[id]->push_core_to_bus_q(reqTr);
}

void CPU :: push_core_to_bus_resp_q ( core_to_bus_tr reqTr, int id){
    m_core_ptr[id]->push_core_to_bus_resp_q(reqTr);
}

void CPU :: push_bus_to_core_q (bus_to_core_tr reqTr, int id){
    m_core_ptr[id]->push_bus_to_core_q(reqTr);
}

void CPU :: pop_core_to_bus_q (int id){
    m_core_ptr[id]->pop_core_to_bus_q();
}

void CPU :: pop_core_to_bus_resp_q (int id){
    m_core_ptr[id]->pop_core_to_bus_resp_q();
}

void CPU :: pop_bus_to_core_q(int id){
    m_core_ptr[id]->pop_bus_to_core_q();
}

core_to_bus_tr CPU :: get_front_core_to_bus_q(int id){
    return m_core_ptr[id]->get_front_core_to_bus_q();
}

core_to_bus_tr CPU :: get_front_core_to_bus_resp_q(int id){
    return m_core_ptr[id]->get_front_core_to_bus_resp_q();
}

bus_to_core_tr CPU :: get_front_bus_to_core_q(int id){
    return m_core_ptr[id]->get_front_bus_to_core_q();
}

int CPU :: get_size_bus_to_core_q(int id){
    return m_core_ptr[id]->get_size_bus_to_core_q();
}

int CPU :: get_size_core_to_bus_q(int id){
    return m_core_ptr[id]->get_size_core_to_bus_q();
}

int CPU :: get_size_core_to_bus_resp_q (int id){
    return m_core_ptr[id]->get_size_core_to_bus_resp_q();
}

int CPU :: get_size_core_inst_q (int id){
    return m_core_ptr[id]->get_size_inst_q();
}

void CPU :: printPerf (){
    for (int i=0; i<8; i++){
        m_core_ptr[i]->printPerf();
    }
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

            inst.id = stoi(v[0]);
            inst.op = v[1];
            inst.address = stoi (v[2]);
            inst.data = stoi(v[3]);
            inst.coreID = stoi(v[4]);
            inst.valid = true;

            cout << "Indtructions to be executed:\n";
            print_instruction (inst);

            assert (inst.coreID < 8);
            m_core_ptr[inst.coreID]->push_inst_q(inst);
            //inst_q.push (inst);
            
            v.clear();
        }
        
        /*
        cout << "Line: " << line << "\n";
        getInst (line, v);
        cout << " cpu :: load_inst_q : vector size: " << v.size() << "\n";
        print_vector (v);
        */
    } else {
        cout << "File couldn't be opened!!\n";
    }

    input_file.close();
}

void CPU :: run_function (){
    //load_inst_q (inputFile);

    for (int i=0; i<8; i++){
        //cout << " CPU:: inst_q size of core id: " << i << " is " << m_core_ptr[i]->get_size_inst_q() << "\n";
        m_core_ptr[i]->run_function();
    }


}