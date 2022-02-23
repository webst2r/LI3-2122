#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <time.h>
#include "../include/sgg.h"

int main(int argc, char*argv[]){
    if(argc < 2){
        perror("Argumentos Insuficientes");
        return 0;
    }
    else {
    SGG sgg =load_sgg("entrada/users-g2.csv","entrada/commits-g2.csv","entrada/repos-g2.csv");
    char a[30]="entrada/";
    strcat(a,argv[1]);
    lerFicheiroQueries(a,sgg);
    return 1;}
}