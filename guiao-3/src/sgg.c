#include "../include/sgg.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define LINE_SIZE 1000

struct sgg {
    C_users users;
    C_commits commits;
    C_repos repos;
};

SGG init_sgg(){
    SGG nSGG = malloc(sizeof(struct sgg));
    nSGG->users = init_users(nSGG->users);
    nSGG->commits = init_commits(nSGG->commits);
    nSGG->repos = init_repos(nSGG->repos);
    return nSGG;
}
void free_sgg(SGG sgg){
    free_users(sgg->users);
    free_repos(sgg->repos);
}


void set_SGG(SGG sgg, C_users users,C_commits commits,C_repos repos){
    sgg->users = users;
    sgg->commits = commits;
    sgg->repos = repos;
}

//   QUERIES
struct gh_query{
    int numero; 
    int N; 
    char *data_inicio;
    char *data_fim;
    char *language;
};

//   QUERIES ESTATÍSTICAS 

// Query1: 
void query1(FILE *f) {
    fprintf(f,"Bot: %d\nOrganization: %d\nUser: %d\n\n",get_bot(),get_organization(),get_user());
}

void printQuery1(){
    printf("————————————————————————————————————————————————\n");
    printf("| Bot: %d | Organization: %d | User: %d |\n",get_bot(),get_organization(),get_user());
    printf("————————————————————————————————————————————————\n");
}

//Query2
void query2(SGG sgg,FILE *f) {
    float f1= get_NColaboradores();
    float f2= get_Size_Repos(sgg->repos);
    fprintf(f,"%.2f\n\n",f1/f2);
}

void printQuery2(SGG sgg){
    float f1= get_NColaboradores();
    float f2= get_Size_Repos(sgg->repos);
    printf("————————————————————————————————————————————————\n");
    printf("| Nº médio colaboradores por repositório: %.2f |\n",f1/f2);
    printf("————————————————————————————————————————————————\n");
}


//Query3
void query3(FILE *f) {fprintf(f,"%d\n\n",get_Repos_with_Bots());}

void printQuery3(){
    printf("————————————————————————————————————\n");
    printf("| Nº repositórios com bots: %d |\n",get_Repos_with_Bots());
    printf("————————————————————————————————————\n");
}

//Query4
void query4(SGG sgg,FILE *f) {
    float f1 = get_nCommits();
    float f2 = get_Size_User(sgg->users);
    fprintf(f,"%.2f\n\n",f1/f2);
}

void printQuery4(SGG sgg){
    float f1 = get_nCommits();
    float f2 = get_Size_User(sgg->users);
    printf("—————————————————————————————————————————\n");
    printf("| Média de commits por utilizador: %.2f |\n",f1/f2);
    printf("—————————————————————————————————————————\n");

}

//Query5
GArray* query5(SGG sgg,int N,char *dataInicio,char *dataFim,FILE *f, int tipo){
    GArray* arrayStrings = g_array_new(FALSE, FALSE, sizeof(char*));
    arrayStrings = calculaUtilizadoresMaisAtivos(sgg->commits,sgg->users,N,dataInicio,dataFim,f,tipo);
    return arrayStrings;
}

// Query6
GArray* query6(SGG sgg,int N,char *language, FILE *f, int tipo){
    GArray* arrayStrings = g_array_new(FALSE, FALSE, sizeof(char*));
    arrayStrings = calculaUtilizadoresMaisCommitsLinguagem(sgg->commits,sgg->users,sgg->repos,N,language,f,tipo);
    return arrayStrings;
}

//Query7
GArray* query7(SGG sgg,char *dataInicio,FILE *f, int tipo){
     GArray* arrayStrings = g_array_new(FALSE, FALSE, sizeof(char*));
     arrayStrings = determinaReposInativos(sgg->commits,sgg->repos,dataInicio,f,tipo);
     return arrayStrings;
}




//Query8
GArray* query8(SGG sgg,int N,char *dataInicio,FILE *f, int tipo){
    GArray* arrayStrings = g_array_new(FALSE, FALSE, sizeof(char*));
    arrayStrings = determinaLinguagensMaisUtilizadas(sgg->repos,N,dataInicio,f,tipo);
    return arrayStrings;

}



//Query9
GArray* query9(SGG sgg,int N,FILE *f, int tipo){
    GArray* arrayStrings = g_array_new(FALSE, FALSE, sizeof(char*));
    arrayStrings = determinaTopUsersAmigoOwner(sgg->commits,sgg->repos,sgg->users,N,f,tipo);
    return arrayStrings;
}


//Query10
GArray* query10(SGG sgg,int N,FILE *f,int tipo){
    GArray* arrayStrings = g_array_new(FALSE, FALSE, sizeof(char*));
    arrayStrings = determinaUtilizadoresMaiorMsg(sgg->users,sgg->commits,N,f,tipo);
    return arrayStrings;
}





char* toLower(char* s) {
  for(char *p=s; *p; p++) *p=tolower(*p);
  return s;
}
//parsing
GH_QUERY build_query(char *line){
    GH_QUERY query = malloc(sizeof(struct gh_query));
    char *buffer2 = line;

    // Numero da Query
    buffer2=strdup(strsep(&line, " "));
    query -> numero = atoi(buffer2);

    if(query->numero == 5){
        buffer2=strdup(strsep(&line," "));
        query->N = atoi(buffer2);
        query->data_inicio = strdup(strsep(&line, " "));
        query->data_fim = strdup(strsep(&line, ""));
        return query;
    } else if (query->numero == 6){
        buffer2=strdup(strsep(&line," "));
        query->N = atoi(buffer2);
        char *lang = strdup(strsep(&line,""));
        lang[strcspn(lang, "\r\n")] = 0; // remover o \n que nao era suposto ficar no fim
        query->language = toLower(lang);
    } else if (query->numero == 7){
        query->data_inicio = strdup(strsep(&line," "));
    } else if (query->numero == 8){
        buffer2=strdup(strsep(&line," "));
        query->N = atoi(buffer2);
        query->data_inicio=strdup(strsep(&line," "));
    } else if (query->numero == 9 || query->numero == 10){
        buffer2=strdup(strsep(&line," "));
        query->N = atoi(buffer2);
    }
    return query;
}

void interpretador(GH_QUERY q,SGG sgg,FILE *f){
    switch (q->numero) {
        case 1:
            query1(f);
            break;
        case 2:
            query2(sgg,f);
            break;
        case 3:
            query3(f);
            break;
        case 4:
            query4(sgg,f);
            break;
        case 5:
            query5(sgg,q->N,q->data_inicio,q->data_fim,f,0);
            break;
        
        case 6:
            query6(sgg,q->N,q->language,f,0);
            break;
        
        case 7:
            query7(sgg,q->data_inicio,f,0);
            break;
        
        case 8:
            query8(sgg,q->N,q->data_inicio,f,0);
            break;
        
        case 9:
            query9(sgg,q->N,f,0);
            break;
        case 10:
            query10(sgg,q->N,f,0);
            break;
        
        default:
            printf("ERROR! Ocorreu um erro na leitura do ficheiro das querry!\n");
    }
}

void lerFicheiroQueries(char *fileQueries,SGG sgg){
    char buff[LINE_SIZE];
    FILE *fQ = fopen(fileQueries,"r");
    if(fQ == NULL){
        perror("Erro ao abrir ficheiro:");
        exit(EXIT_FAILURE);
    }
    int i=1;
    char *name="saida/command";
    char *fim="_output.txt";
    char num[20];
    
    while(fgets(buff,LINE_SIZE,fQ)){
        char r[35]="";
        sprintf(num, "%d", i); 
        strcat(r,name);
        strcat(r,num);
        strcat(r,fim);
        FILE *f = fopen(r,"w");
        GH_QUERY q = build_query(buff);
        interpretador(q,sgg,f);
        i++;
        fclose(f);
    }
    fclose(fQ);
}
