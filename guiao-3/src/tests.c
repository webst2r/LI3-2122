#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/commits.h"
#include "../include/repos.h"
#include "../include/user.h"
#include "../include/sgg.h"
#include "../include/view.h"
#include "../include/validate.h"


void compareFiles(FILE *fp1, FILE *fp2){

    char ch1 = getc(fp1);
    char ch2 = getc(fp2);

    int error = 0, pos = 0, line = 1;

    while (ch1 != EOF && ch2 != EOF){
        pos++;
        if (ch1 == '\n' && ch2 == '\n'){
            line++;
            pos = 0;
        }

        if (ch1 != ch2){
            error++;
            printf("Erro: Linha %d Pos %d \n", line, pos);
        }
        ch1 = getc(fp1);
        ch2 = getc(fp2);
    }
  
    if(error == 0) printf("O ficheiro produzido é idêntico ao ficheiro expected.\n\n");
    else printf("Os ficheiros não são iguais.\n\n");
}


void compare(FILE *f, char *produced_file){
    FILE *g = fopen(produced_file,"r");
    if(g == NULL) {
        printErroAbrirFicheiro();
        exit(EXIT_FAILURE);
    }

    compareFiles(f,g);
    fclose(f);
    fclose(g);
}

// avaliar se é executada em tempo útil (< 5seg) 
// avaliar se o resultado é correto.
void test_query(SGG sgg){
    clock_t start, end;
    double cpu_time_used;
    
    FILE *f1 = fopen("saida/query1_produced.txt","w");
    FILE *f2 = fopen("saida/query2_produced.txt","w");
    FILE *f3 = fopen("saida/query3_produced.txt","w");
    FILE *f4 = fopen("saida/query4_produced.txt","w");
    FILE *f5 = fopen("saida/query5_produced.txt","w");
    FILE *f6 = fopen("saida/query6_produced.txt","w");
    FILE *f7 = fopen("saida/query7_produced.txt","w");
    FILE *f8 = fopen("saida/query8_produced.txt","w");
    FILE *f9 = fopen("saida/query9_produced.txt","w");
    FILE *f10 = fopen("saida/query10_produced.txt","w");
    

    if(f1 == NULL || f2 == NULL || f3 == NULL || f4 == NULL || f5 == NULL || f6 == NULL ||f7 == NULL ||f8 == NULL || f9 == NULL || f10 == NULL) {
        printErroAbrirFicheiro();
    }

    
    // Query 1
    start = clock();
    query1(f1);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printCPUTime(1,cpu_time_used);
    compare(f1,"expected/query1_expected.txt");


    // Query 2
    start = clock();
    query2(sgg,f2);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printCPUTime(2,cpu_time_used);
    compare(f2,"expected/query2_expected.txt");

    // Query 3
    start = clock();
    query3(f3);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printCPUTime(3,cpu_time_used);
    compare(f3,"expected/query3_expected.txt");
    // Query 4
    start = clock();
    query4(sgg,f4);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printCPUTime(4,cpu_time_used);
    compare(f4,"expected/query4_expected.txt");
    
    // Query 5
    start = clock();
    query5(sgg,100,"2010-01-01","2015-01-01",f5,0);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printCPUTime(5,cpu_time_used);
    compare(f5,"expected/query5_expected.txt");
    
    // Query 6
    start = clock();
    char lang[20] = "python";
    query6(sgg,100,lang,f6,0);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printCPUTime(6,cpu_time_used);
    compare(f6,"expected/query6_expected.txt");
          
    // Query 7
    start = clock();
    query7(sgg,"2010-01-01",f7,0);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printCPUTime(7,cpu_time_used);
    compare(f7,"expected/query7_expected.txt");

    // Query 8   
    start = clock();
    query8(sgg,100,"2010-01-01",f8,0);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printCPUTime(8,cpu_time_used);
    compare(f8,"expected/query8_expected.txt");
        
    // Query 9
    start = clock();
    query9(sgg,100,f9,0);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printCPUTime(9,cpu_time_used);
    compare(f9,"expected/query9_expected.txt");

    // Query 10
    start = clock();
    query10(sgg,3,f10,0);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printCPUTime(10,cpu_time_used);
    compare(f10,"expected/query10_expected.txt");
    
}


int main(int argc, char* argv[]){
    
    SGG sgg = validateAndStart();
    printLowerLoadingTests();

    test_query(sgg);
    
    return 0;
}