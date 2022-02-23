#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "../libs/repos.h"
#include "../libs/valid_date.h"
#include <sys/stat.h>

#define LINE_SIZE 1000000

struct gh_repos {
    char *license;
    char *description;
    char *language;
    char *full_name;
    char *default_branch;
    struct tm created_at;
    struct tm updated_at;
    int forks_count;
    int open_issues;
    int stargazers_count;
    int owner_id;
    int id;
    int size;
    bool has_wiki;
};

int reposIsValid (GH_REPOS rep) {
    if (rep->forks_count<0 || rep->open_issues<0 || rep->stargazers_count<0 || rep->owner_id<=0 || rep->id<=0 || rep->size<0) {return 0;}
    if(strcmp(rep->license,"")==0 || strcmp(rep->language,"")==0 || strcmp(rep->full_name,"")==0 || strcmp(rep->default_branch,"")==0) {return 0;}
    return 1;
}
int get_owner_id (GH_REPOS r) {return r->owner_id; }

int add_repos2file(GH_REPOS rep, FILE *f) {
    char str_created_at[20];
    char str_updated_at[20];
    char *str_bool;
    strftime(str_created_at,sizeof(str_created_at),"%Y-%m-%d %H:%M:%S",&(rep->created_at));
    strftime(str_updated_at,sizeof(str_updated_at),"%Y-%m-%d %H:%M:%S",&(rep->updated_at));
    if(rep->has_wiki == false) str_bool = strdup("False");
    else str_bool = strdup("True");
    fprintf(f,"%d;%d;%s;%s;%s;%s;%s;%s;%s;%s;%d;%d;%d;%d\n",rep->id, rep->owner_id,rep->full_name, rep->license, str_bool,rep->description,rep->language,rep->default_branch,str_created_at,str_updated_at,rep->forks_count,rep->open_issues,rep->stargazers_count,rep->size);
    return 0;
}

void print_repos (GH_REPOS rep){
    char str_created_at[20];
    char str_updated_at[20];
    char *str_bool;
    strftime(str_created_at,sizeof(str_created_at),"%Y-%m-%d %H:%M:%S",&(rep->created_at));
    strftime(str_updated_at,sizeof(str_updated_at),"%Y-%m-%d %H:%M:%S",&(rep->updated_at));
    if(rep->has_wiki == false) str_bool = strdup("False");
    else str_bool = strdup("True");
    printf("%d;%d;%s;%s;%s;%s;%s;%s;%s;%s;%d;%d;%d;%d\n",rep->id, rep->owner_id,rep->full_name, rep->license, str_bool,rep->description,rep->language,rep->default_branch,str_created_at,str_updated_at,rep->forks_count,rep->open_issues,rep->stargazers_count,rep->size);
}


GH_REPOS build_repos(char* line){
    GH_REPOS repos = malloc(sizeof(struct gh_repos));
    char* buffer2 = line;

    /* ID*/    
    buffer2=strdup(strsep(&line, ";\n"));
    if(strcmp(buffer2, "")!=0) {
    repos->id = atoi(buffer2);}
    else{repos->id = -1;}

    /* OWNER_ID */
    buffer2=strdup(strsep(&line, ";\n"));
    if(strcmp(buffer2, "")!=0) {
    repos->owner_id = atoi(buffer2);}
    else{repos->owner_id = -1;}

    /* FULL NAME */
    repos->full_name = strdup(strsep(&line,";\n"));

    /* LICENSE */
    repos->license = strdup(strsep(&line,";\n"));

    /* HAS_WIKI*/
    buffer2=strdup(strsep(&line,";\n"));
    if(strcmp(buffer2,"")==0) repos->id = -1;
    else if(strcmp(buffer2, "True") == 0) repos->has_wiki = true;
    else repos->has_wiki = false;

    /* DESCRIPTION */
    repos->description = strdup(strsep(&line,";\n"));

    /* LANGUAGE */
    repos->language = strdup(strsep(&line,";\n"));

    /* DEFAULT_BRANCH */
    repos->default_branch = strdup(strsep(&line,";\n"));

    /* CREATED_AT*/

    char *str_data = strdup(strsep(&line,";\n"));
    int year, month, day, hour, minutes, seconds;
    int nc[6] = { 0 };
    sscanf(str_data, "%*[0-9]%n-%*[0-9]%n-%*[0-9]%n %*[0-9]%n:%*[0-9]%n:%*[0-9]%n", &nc[0], &nc[1], &nc[2],&nc[3],&nc[4],&nc[5]); 
    if (nc[0] != 4 || nc[1] != 7 || nc[2] != 10 || nc[3] != 13 || nc[4] != 16 || nc[5] != 19){repos->id = -1;}
    sscanf(str_data, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minutes,&seconds);
    
    if (strlen(str_data) != 19 || !(ValidDateFormat(year, month, day))){repos->id = -1;}
    else {
        int ano = atoi(strsep(&str_data, "-"));
        repos->created_at.tm_year = ano - 1900;
        int mes = atoi(strsep(&str_data,"-"));
        repos->created_at.tm_mon = mes -1;
        repos->created_at.tm_mday = atoi(strsep(&str_data," "));
        repos->created_at.tm_hour = atoi(strsep(&str_data, ":"));
        repos->created_at.tm_min = atoi(strsep(&str_data, ":"));
        repos->created_at.tm_sec = atoi(strsep(&str_data, ""));
    }
    if(!(ValidDate(repos->created_at))){repos->id = -1;}

    /* UPDATED_AT*/
    char *str_data2 = strdup(strsep(&line,";\n"));
    int year2, month2, day2, hour2, minutes2, seconds2;
    int nv[6] = { 0 };
    sscanf(str_data2, "%*[0-9]%n-%*[0-9]%n-%*[0-9]%n %*[0-9]%n:%*[0-9]%n:%*[0-9]%n", &nv[0], &nv[1], &nv[2],&nv[3],&nv[4],&nv[5]); 
    if (nv[0] != 4 || nv[1] != 7 || nv[2] != 10 || nv[3] != 13 || nv[4] != 16 || nv[5] != 19){repos->id = -1;}
    sscanf(str_data2, "%d-%d-%d %d:%d:%d", &year2, &month2, &day2, &hour2, &minutes2,&seconds2);
    
    if (strlen(str_data2) != 19 || !(ValidDateFormat(year2, month2, day2))){repos->id = -1;} 
    else {
        int ano = atoi(strsep(&str_data2, "-"));
        repos->updated_at.tm_year = ano - 1900;
        int mes = atoi(strsep(&str_data2,"-"));
        repos->updated_at.tm_mon = mes -1;
        repos->updated_at.tm_mday = atoi(strsep(&str_data2," "));
        repos->updated_at.tm_hour = atoi(strsep(&str_data2, ":"));
        repos->updated_at.tm_min = atoi(strsep(&str_data2, ":"));
        repos->updated_at.tm_sec = atoi(strsep(&str_data2, ""));
    }
    if(!(ValidDate(repos->updated_at))){repos->id = -1;}

    /* FORKS_COUNT */
    buffer2=strdup(strsep(&line, ";\n"));
    if(strcmp(buffer2, "")!=0) {
    repos->forks_count = atoi(buffer2);}
    else{repos->forks_count = -1;}

    /* OPEN_ISSUES */
    buffer2=strdup(strsep(&line, ";\n"));
    if(strcmp(buffer2, "")!=0) {
    repos->open_issues = atoi(buffer2);}
    else{repos->open_issues = -1;}

    /* STARGAZERS_COUNT */
    buffer2=strdup(strsep(&line, ";\n"));
    if(strcmp(buffer2, "")!=0) {
    repos->stargazers_count = atoi(buffer2);}
    else{repos->stargazers_count = -1;}

    /* SIZE */
    buffer2=strdup(strsep(&line, ";\n"));
    if(strcmp(buffer2, "")!=0) {
    repos->size = atoi(buffer2);}
    else{repos->size = -1;}


    return repos;
}

//Funçao que dá free a um user

void freeRepos(GH_REPOS r){
    free(r->license);
    free(r->description);
    free(r->language);
    free(r->full_name);
    free(r->default_branch);
    free(r);
}

//Função que remove um user da hashTable
void rmRepos(gpointer res){
    GH_REPOS r = res;
    freeRepos(r);
    return;
}

//Função que dá free a uma chave
void rmKeyRepos(gpointer key){
    free((int*)key);
}

GHashTable * load_repos (char *in, char *out){
    GH_REPOS rep;
    char buff[LINE_SIZE];
    char ch;
    FILE *f = fopen(in,"r");
    FILE *g = fopen(out,"w");
    int *key;
    int validos = 0;
    GHashTable * repos  = g_hash_table_new_full(g_int_hash,g_int_equal,rmKeyRepos,rmRepos);
    

    if(f == NULL || g == NULL){
        perror("Error: ");
        exit(EXIT_FAILURE);
    }

    while((ch = fgetc(f)) != EOF){
        putc(ch, g);
        if(ch=='\n') break;
    }
    
    printf("Ficheiro %s a processar...\n",in);
        while(fgets(buff,LINE_SIZE,f)){
            rep = build_repos(buff);
            if(reposIsValid(rep)) {
                add_repos2file(rep,g);
                key = &(rep->id);
                g_hash_table_insert(repos,key,rep);
                validos++;                
            }
        }
    fclose(f);
    fclose(g);
    printf("Ficheiro lido e verificado\n");
    printf("Registos Validos em %s: %d\n\n",in,validos);
    return repos;
}