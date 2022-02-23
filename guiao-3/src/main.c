#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <time.h>
#include "../include/commits.h"
#include "../include/repos.h"
#include "../include/user.h"
#include "../include/sgg.h"
#include "../include/view.h"
#include "../include/validate.h"


int main(int argc, char*argv[]){
    
    SGG sgg = validateAndStart();
    printLowerLoading();
    char enter = 0;
    while (enter != '\r' && enter != '\n') { enter = getchar(); }
    int sysret = system("clear");


    if(argc < 2 && sysret != -1){
        int option = -1,n,numeroPaginas;
        char dataInicio[15], dataFim[15], linguagem[35];
        FILE *f =  NULL;
        GArray* arrayStrs5, *arrayStrs4, *arrayStrs3, *arrayStrs2, *arrayStrs, *arrayStrs6;

        printMenu();
        printInsiraOpcao();

        while(scanf("%d",&option) > 0 && option != 0){

            switch(option) {
            case 1:
                printQuery1();
                break;
            case 2:
                printQuery2(sgg);
                break;
            case 3:
                printQuery3();
                break;
            case 4:
                printQuery4(sgg);
                break;
            case 5:
                printNumeroUtilizadores();
                numeroPaginas = scanf("%d",&n);
                printInsiraData(0);
                numeroPaginas = scanf("%s",dataInicio);
                printInsiraData(1);
                numeroPaginas = scanf("%s",dataFim);
                
                numeroPaginas = n / MAXPAGE;
                if(n % MAXPAGE > 0) numeroPaginas += 1;

                arrayStrs = query5(sgg,n,dataInicio,dataFim,f,99);
                paginacao(arrayStrs,1,numeroPaginas);
                selector(numeroPaginas, arrayStrs);
                break;

            
            case 6:
                printNumeroUtilizadores();
                numeroPaginas = scanf("%d",&n);
                printInsiraLinguagem();
                numeroPaginas = scanf("%s",linguagem);
                
                numeroPaginas = n / MAXPAGE;
                if(n % MAXPAGE > 0) numeroPaginas += 1;

                arrayStrs2 = query6(sgg,n,toLower(linguagem),f,99);
                paginacao(arrayStrs2,1,numeroPaginas);
                selector(numeroPaginas, arrayStrs2);

                break;

            
            case 7:
                printInsiraData(3);
                numeroPaginas = scanf("%s",dataInicio);

                arrayStrs6 = query7(sgg,dataInicio,f,99);
                numeroPaginas = arrayStrs6->len / MAXPAGE;
                if(n % MAXPAGE > 0) numeroPaginas += 1;

                paginacao(arrayStrs6,1,numeroPaginas);
                selector(numeroPaginas, arrayStrs6);
                break;
            
            case 8:
                printNumeroLinguagens();
                numeroPaginas = scanf("%d",&n);
                printInsiraData(3);
                numeroPaginas = scanf("%s",dataInicio);

                numeroPaginas = n / MAXPAGE;
                if(n % MAXPAGE > 0) numeroPaginas += 1;

                arrayStrs3 = query8(sgg,n,dataInicio,f,99);
                paginacao(arrayStrs3,1,numeroPaginas);
                selector(numeroPaginas, arrayStrs3);
                break;
            
            case 9:
                printNumeroUtilizadores();
                numeroPaginas = scanf("%d",&n);

                numeroPaginas = n / MAXPAGE;
                if(n % MAXPAGE > 0) numeroPaginas += 1;

                arrayStrs4 = query9(sgg,n,f,99);
                paginacao(arrayStrs4,1,numeroPaginas);
                selector(numeroPaginas, arrayStrs4);
                break;
            
            case 10:
                printNumeroUtilizadores();
                numeroPaginas = scanf("%d",&n);
            
                arrayStrs5 = query10(sgg,n,f,99);
                numeroPaginas = arrayStrs5->len / MAXPAGE;
                if(n % MAXPAGE > 0) numeroPaginas += 1;
            
                paginacao(arrayStrs5,1,numeroPaginas);
                selector(numeroPaginas, arrayStrs5);
                break;
            default:
                printOpcaoInvalida();   
            }
            printMenu();
            printInsiraOpcao();
        }
        limparTerminal();
        return 0;
        
        } else {
        char a[30]="entrada/";
        strcat(a,argv[1]);
        lerFicheiroQueries(a,sgg);
        return 1;
        }
}