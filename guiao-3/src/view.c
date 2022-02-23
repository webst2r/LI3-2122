#include "../include/view.h"
#include"../include/sgg.h"

void printErroAbrirFicheiro(){
    printf("Erro ao abrir o ficheiro.\n");
}

void printCPUTime(int query,double cpu_time_used){
    printf("cpu_time query %d: %fs\n",query,cpu_time_used);
}

void printInsiraLinguagem(){
    printf("\nInsira a linguagem: ");
}

void printNumeroLinguagens(){
    printf("\n\nInsira número de linguagens: ");
}

void limparTerminal(){
    printf("\e[1;1H\e[2J");
}

void printInsiraOpcao(){
    printf("\n\nInsira a opção (digite 0 para sair): ");
}

void printInsiraData(int i){
    if(i == 0) printf("\nInsira a data de início:(YYYY-MM-DD): ");
    else if(i == 1) printf("\nInsira a data de fim:(YYYY-MM-DD): ");
    else printf("\nInsira a data:(YYYY-MM-DD): ");
}

void printNumeroUtilizadores(){
    printf("\n\nInsira número de utilizadores: ");
}

void printOpcaoInvalida(){
    printf("Opcao Invalida.\n"); 
}

void printMenu(){
     printf("\n\n************************ MENU *************************\n");
    printf("———————————————————————————————————————————————————————\n");
    printf("| 1 | Quantidade de bots, organizações e utilizadores |\n");
    printf("———————————————————————————————————————————————————————\n");
    printf("———————————————————————————————————————————————————————\n");
    printf("| 2 | Número médio de colaboradores por repositório   |\n");
    printf("———————————————————————————————————————————————————————\n");
    printf("———————————————————————————————————————————————————————\n");
    printf("| 3 | Quantidade de repositórios com bots             |\n");
    printf("———————————————————————————————————————————————————————\n");
    printf("———————————————————————————————————————————————————————\n");
    printf("| 4 | Quantidade média de commits por utilizador      |\n");
    printf("———————————————————————————————————————————————————————\n");
    printf("———————————————————————————————————————————————————————\n");
    printf("| 5 | Top N utilizadores mais ativos entre datas      |\n");
    printf("———————————————————————————————————————————————————————\n");
    printf("——————————————————————————————————————————————————————————\n");
    printf("| 6 | Top N utilizadores com mais commits em repositórios|\n|     de uma linguagem\t\t\t\t\t |\n");
    printf("——————————————————————————————————————————————————————————\n");
    printf("—————————————————————————————————————————————————————————\n");
    printf("| 7 | Lista de repositórios sem commits a partir de data|\n");
    printf("—————————————————————————————————————————————————————————\n");
    printf("—————————————————————————————————————————————————————————\n");
    printf("| 8 | Top N linguagens mais utilizadas a partir de data |\n");
    printf("—————————————————————————————————————————————————————————\n");
    printf("——————————————————————————————————————————————————————————\n");
    printf("| 9 | Top N utilizadores com mais commits em repositórios|\n      cujo owner é amigo seu.\t\t\t\t |\n");
    printf("——————————————————————————————————————————————————————————\n");
    printf("——————————————————————————————————————————————————————————\n");
    printf("| 10 | Top N utilizadores com maiores mensagens de commit|\n|      por repositório.\t\t\t\t\t |\n");
    printf("——————————————————————————————————————————————————————————\n");
    printf("\n----------------------------------------------------------");
 }


void printLowerLoadingTests() {
    printf("                     ╚════════════════════════════════════════════════════════════════════════════════════════════╝");
    printf("\n \033[0m");
}

void printLowerLoading(){
    printf("                     ╚════════════════════════════════════════════════════════════════════════════════════════════╝");
    printf("\n\n \033[0;33m Press Enter to Continue \033[0m");
}
void paginacao(GArray* arrStr,int p, int total) {
    int inicio = (p-1) * MAXPAGE;
    int fim = inicio + MAXPAGE;
    int i, size = arrStr->len;
    
    printf("———————————————————————————————————————————————————————\n");

    if(inicio < size && fim < size){
        for(i = 0; inicio < fim; inicio++,i++){
            printf("%s\n",g_array_index(arrStr, char*, inicio));
        }
    } else if(inicio < size){
        for(i = 0; inicio < size; inicio++,i++){
            printf("%s\n",g_array_index(arrStr, char*, inicio));
        }
    }

    printf("\n———————————————————————————————————————————————————————\n");
    printf("——————————————————Página \033[0;31m%d\033[0m de %d——————————————————\n",p,total);
    printf("P      -> Próxima\n");
    printf("A      -> Anterior\n");
    printf("S<N>   -> Saltar para página\n");
    printf("F      -> Sair desta query\n");

}

void selector(int numeroPaginas, GArray *arrayStrs){
    int paginaAtual = 1;
    char opcaoPagina[5];
    int di = scanf("%s", opcaoPagina);
    
    while((strcmp(opcaoPagina,"F")) != 0){
                if((strcmp(opcaoPagina,"P") == 0) && paginaAtual+1 <= numeroPaginas){
                    paginaAtual += 1;
                    printf("\e[1;1H\e[2J");
                    paginacao(arrayStrs, paginaAtual,numeroPaginas);
                } else if((strcmp(opcaoPagina,"A") == 0) && paginaAtual > 1){
                    paginaAtual -= 1;
                    printf("\e[1;1H\e[2J");
                    paginacao(arrayStrs,paginaAtual,numeroPaginas);
                } 
                
                else if(opcaoPagina[0] == 'S' &&  (atoi(opcaoPagina+1) <= numeroPaginas)){
                    di = atoi(opcaoPagina+1);
                    paginaAtual = di;
                    printf("\e[1;1H\e[2J");
                    paginacao(arrayStrs,paginaAtual,numeroPaginas);
                } else printf("Opcao Invalida.\n");
                di = scanf("%s", opcaoPagina);
            }
    
}