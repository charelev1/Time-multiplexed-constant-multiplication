#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cassert>

void assert_log(std::string msg){
     std::cout << msg <<"\n";
     assert(0);
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
