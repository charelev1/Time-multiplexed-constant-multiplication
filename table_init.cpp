#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cassert>

#define pair_int std::pair<int,int>

void assert_log(std::string msg){
     std::cout << msg <<"\n";
     assert(0);
}

                                                                               
struct table_t {                                                                
    /* struct of a row of coefficient tablei */
    pair_int coeff; // number and cost   
    std::vector<pair_int> all_parents_set; // all different parent funamentals set + cost
    std::vector<std::pair<pair_int, pair_int> parent_fund_set; // parent funamentals sets
    std::vector<std::pair<double, double>> edge_set; // edge values sets  
};                                                                              

void is_valid_coeff(pair_int coeff, std::pair<pair_int, pair_int> parent_nodes,
        std::pair<double, double> shifts, std::vector<table_t>& coeff_table) {
    /*  check if candidate parents make the cn coefficient */                      

    int cn = coeff.first;                                                       
    int cost = coeff.second;
    int max_coeff = coeff_table.size() / 2 - 1;                                 
                                                                                
    // check if the canditate coefficient is >1 and < max_num, ... 
    if (cn>1 && cn<=max_coeff && (cn%2==1) && cn!=parent_nodes.first.num &&
        cn!=parent_nodes.second.num) {
        // the index of the coefficient in the table
        int fidx = find_table_index(coeff, coeff_table);               
        bool flag = 0;                                                          
        // check for duplicates
        for (int i = 0; i < coeff_table[fidx].edge_set.size(); i++) {           
            auto parent_pair = coeff_table[fidx].parent_fund_set[i];            
            auto edge_pair = coeff_table[fidx].edge_set[i];                     
            // maybe here bug                                                   
            if ((parent_pair == parent_nodes || 
                    parent_pair == swap_pair<pair_int>(parent_nodes)) &&
                (edge_pair == shifts ||
                    edge_pair == swap_pair<double>(shifts))) {
                flag = 1;                                                       
                break;                                                          
            }                                                                   
        }                                                                       
                                                                                
        //append if no duplicates are found
        if (!flag) {                                                            
            coeff_table[fidx].coeff.second = cost; //CAN I DELETE THIS???
            coeff_table[fidx].parent_fund_set.push_back(parent_nodes);          
            coeff_table[fidx].edge_set.push_back(shifts);                       
        }                                                                       
    }                                                                           
}                                                                               


void children_search(pair_int parent_left, pair_int parent_right, int shift, 
        int cost, std::vector<table_t>& coeff_table) {
    /* find all the possible (odd) children from the two input parents (left and
     * right given a shift value and a cost */

    // there are 9 possible children 
    pair_int c1;
    pair_int c2;                                                                   
    pair_int c3;                                                                   
    pair_int c4;                                                                   
    pair_int c5;                                                                   
    pair_int c6;                                                                   
    pair_int c7;                                                                   
    pair_int c8;                                                                   
    pair_int c9;                                                                   

    c1.first =  parent_left.first + shift * parent_right.first;                                           
    c2.first =  parent_left.first - shift * parent_right.first;                                           
    c3.first = -parent_left.first + shift * parent_right.first;                                          
    c4.first =  shift * parent_left.first + parent_right.first;                                           
    c5.first = -shift * parent_left.first + parent_right.first;                                          
    c6.first =  shift * parent_left.first - parent_right.first;                                           
    c7.first = ( parent_left.first + parent_right.first) / shift;                                         
    c8.first = ( parent_left.first - parent_right.first) / shift;                                         
    c9.first = (-parent_left.first + parent_right.first) / shift;                                        

    c1.second = cost;                                                             
    c2.second = cost;                                                             
    c3.second = cost;                                                             
    c4.second = cost;                                                             
    c5.second = cost;                                                             
    c6.second = cost;                                                             
    c7.second = cost;                                                             
    c8.second = cost;                                                             
    c9.second = cost;                                                             

    double dshift = (double)shift;                                              
    is_valid_coeff(c1, {parent_left, parent_right}, {1, dshift}, coeff_table);                    
    is_valid_coeff(c2, {parent_left, parent_right}, {1, -dshift}, coeff_table);                   
    is_valid_coeff(c3, {parent_left, parent_right}, {-1, dshift}, coeff_table);                   
    is_valid_coeff(c4, {parent_left, parent_right}, {dshift, 1}, coeff_table);                    
    is_valid_coeff(c5, {parent_left, parent_right}, {-dshift, 1}, coeff_table);                   
    is_valid_coeff(c6, {parent_left, parent_right}, {dshift, -1}, coeff_table);                   
    is_valid_coeff(c7, {parent_left, parent_right}, {1.0 / dshift, 1.0 / dshift}, coeff_table);   
    is_valid_coeff(c8, {parent_left, parent_right}, {1.0 / dshift, -1.0 / dshift}, coeff_table);  
    is_valid_coeff(c9, {parent_left, parent_right}, {-1.0 / dshift, 1.0 / dshift}, coeff_table);  
}                                                                               

void build_coefficient_table(int max_num, int max_adders, int max_shift,
        std::vector<table_t>& coeff_table){
    /* build the coefficient table of DAGs of up-to max_number that utilize
       up-to max_adders */
    /* Note: check if I can do simplifications, DELETES and any if
       optimizations*/

    /* constant initializations */
    int table_size = max_num>>1; //half size as only the odd numbers are needed
    int adder_cost; //used for more readability
    pair_int c0 = {1,0}; //0th fundamental

    /* intialize table */
    //need a table copy for each additional adder
    coeff_table.resize(table_size*max_adders); 

    for (int j=0; j<max_adders; j++) { //each additional adder
        //init the 1 value as cost of 0
        coeff_table[table_size*j].coeff.first = 1;
        coeff_table[table_size*j].coeff.second = 0;
        //init for all the odd values with the current adder cost
        for (int i=1; i<table_size; i++) {
            coeff_table[i+table_size*a].coeff.first = 2*(i+1)-1;
            coeff_table[i+table_size*a].coeff.second = a+1;
        }
    }
    
    /*find all coefficients that reqire 2 adder*/
    adder_cost = 1;

    for (int i=1; i<max_shift; i++) {// all permitted shifts
        int parent_left = c0;
        int parent_right = c0;
        int shift = 1<<i;
        children_search(c0, c0, shift, adder_cost, coeff_table);
    }


    /*find all coefficients that require 2 adders*/
    adder_cost = 2;                                                             

    for (int i=1; i<max_shift; i++) { // all permitted shifts                       
        int shift = 1<<i;                                                     
        for (int j=1; j<table_size; j++) { // cost 1 numbers               
            if (coeff_table[j].all_parents_set.size() != 0) {                   
                pair_int c1 = coeff_table[j].coeff;                                
                // parents are the same cost 1 fundamental
                children_search(c1, c1, shift, adder_cost, coeff_table);
                // parents are a cost 1 fundamental and the 0th fundamental
                children_search(c1, c0, shift, adder_cost, coeff_table);
            }                                                                   
        }                                                                       
    }                                                                           

    /*find all coefficients that require 3 adders*/
    adder_cost = 3;
                                                                                
    for (int i=1; i<max_shift; i++) { // all permitted shifts
        int shift = 1<<i; 
        // cost 2 fundamentals
        for (int j = table_size+1; j<2*table_size; j++) {
            if (coeff_table[j].all_parents_set.size() != 0) {                   
                pair_int c2 = coeff_table[j].coeff;                                
                // parents are the same cost 2 fundamental
                children_search(c2, c2, shift, adder_cost, coeff_table);
                // parents are a cost 2 fundamental and the 0th fundamental
                children_search(c2, c0, shift, adder_cost, coeff_table);
                // DELETE maybe redundant?? change fidx => j
                int fidx = find_table_index(c2, coeff_table); 
                // parents are a cost 2 fundamental and a cost 1 fundamental
                // that is the child parent of the cost 2 fundamental
                for (auto c1 : coeff_table[fidx].all_parents_set) {            
                    if (c1.second == 1)                                           
                        children_search(c2, c1, shift, adder_cost, coeff_table);
                }                                                               
            }                                                                   
        }                                                                       
        // cost 1 fundamentals
        for (int j = 1; j < table_size; j++) { // cost 1 numbers               
            if (coeff_table[j].all_parents_set.size() != 0) {                   
                for (int i = j; i < table_size; i++) { // cost 1 numbers       
                    if (coeff_table[i].all_parents_set.size() != 0) {           
                        // parents are two any cost 1 fundamentals
                        children_search(coeff_table[i].coeff,
                                coeff_table[i].coeff, shift, adder_cost, coeff_table);
                    }                                                           
                }                                                               
            }                                                                   
        }                                                                       
    }                                                                           

    /*find all coefficients that require 4 adders*/
    adder_cost = 4;

    for (int i=1; i<max_shift; i++) { // all permitted shifts
        int shift = 1<<i;
        // cost 3 fundamentals 
        for (int j = 2*table_size+1; j<3*table_size; j++) {
            if (coeff_table[j].all_parents_set.size() != 0) {
                pair_int c3 = coeff_table[j].coeff;
                // parents are the same cost 3 fundamental
                children_search(c3, c3, shift, adder_cost, coeff_table);
                // parents are a cost 3 fundamental and the 0th fundamental
                children_search(c3, c0, shift, adder_cost, coeff_table);
                // DELETE maybe redundant?? change fidx => j
                int fidx = find_table_index(c3, coeff_table);
                // parents are a cost 3 fundamental and a cost 2 or 1
                // fundamental which is parent of the cost 3 
                for (auto cf : coeff_table[fidx].all_parents_set) {
                    children_search(c3, cf, shift, adder_cost, coeff_table);
                    int fidx2 = find_table_index(cf, coeff_table);
                    // search of a fundamental that is a parent of parent (2
                    // levels below)
                    for (auto ch : coeff_table[fidx2].all_parents_set) { 
                        children_search(c3, ch, shift, adder_cost, coeff_table);
                    }
                }
            }
        }

        // cost 2 fundamentals
        for (int j = table_size + 1; j < 2 * table_size; j++) {  // cost 2 numbers
            if (coeff_table[j].all_parents_set.size() != 0) {
                pair_int c2 = coeff_table[j].coeff;
                // cost 1 fundamentals
                for (int i = 1; i < table_size; i++) {
                    if (coeff_table[i].all_parents_set.size() != 0) {
                        pair_int c1 = coeff_table[i].coeff;
                        children_search(c2, c1, shift, adder_cost, coeff_table);
                    }
                }
                // cost 2 fundamentals
                for (int i = table_size + 1 + j; i < 2 * table_size; i++) {  // cost 2 numbers
                    if (coeff_table[i].all_parents_set.size() != 0) {
                        pair_int c2_bar = coeff_table[i].coeff;
                        // DELETE maybe redundant?? change fidx => j
                        int fidx = find_table_index(c2, coeff_table);
                        int fidx2 = find_table_index(c2_bar, coeff_table);
                        // if c2 and c2_bar have the same fundamental c1
                        for (auto c1 : coeff_table[fidx].all_parents_set) {
                            if (find_in_vec<pair_int>(coeff_table[fidx2].all_parents_set, c1)) {
                                children_search(c2, c2_bar, shift, adder_cost, coeff_table);
                            }
                        }
                    }
                }
            }
        }
    }
}

int main(int argc, char** argv){

    /*search parameters*/
    int Ntm;
    int Npar;
    int Coef_flag;

    /*Initial promts*/
    std::cout<<"Constant multiplication toolbox by Charalampos Eleftheriadis\n";
    for(int i=0; i<argc; i++){
        if(!strcmp(argv[i], "-Ntm")){
            Ntm = atoi(argv[i+1]);
        }else if(!strcmp(argv[i], "-Npar")){
            Npar = atoi(argv[i+1]);
            if (Npar > 2){
                assert_log("Invalid number of parallel outputs. Must be <2");
            }
        }else if(!strcmp(argv[i], "-C")){
            Coef_flag = i+1; 
        }
    }
    printf("\n");

    /*check for the number of arguments*/
    if(Ntm*Npar+5+1!= argc){
        assert_log("Invalid number of inputs arguments!");
    }

    /*read constants*/
    int* coeffs = new int [Npar*Ntm];
    for(int i=0; i<Npar*Ntm; i++){
        coeffs[i] = atoi(argv[Coef_flag+i]);
    }

    /*print the search parameters*/
    printf("Search parameters\n");
    if(Ntm>1){
        printf("Multiplexed constants per output: %d\n", Ntm);
    }
    printf("Parallel outputs: %d\n", Npar);

    for(int j=0; j<Npar; j++){
        printf("Output %d: ", j);
        for(int i=0; i<Ntm; i++){
            printf("%5d ", coeffs[i+j*Ntm]); 
        }
        printf("\n"); 
    }

    delete[] coeffs;
}
