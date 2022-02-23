#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <time.h>
#include "../libs/lerFicheiro.h"

int main(int argc, char*argv[]){
    
    if(argc < 2){
        perror("Argumentos Insuficientes");
        return 1;
    }
    // gcc -Wall lerFicheiro.c user.c commits.c repos.c valid_date.c guiao-1.c -o guiao-1
    // ./guiao-1 exercicio-1
    if(strcmp(argv[1], "exercicio-1")==0){
        exercicio1();
    }
    else if(strcmp(argv[1], "exercicio-2")==0){
        exercicio2();
    }
    return 0;
}
// echo $?    : Diz no terminal qual foi o valor de retorno da Main