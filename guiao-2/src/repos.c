#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "../include/repos.h"
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

struct cat_repos {
    GHashTable *catalogo_repos;
};

int get_OwnerId(GH_REPOS r) {
    return r->owner_id;
}

char * getReposLanguage(GH_REPOS r) {
    return strdup(r->language);
}

int get_Size_Repos (C_repos c) {
    return g_hash_table_size(c->catalogo_repos);
}

GHashTable * get_catalogoRepos(C_repos crepos) {
    return (crepos->catalogo_repos);
}

int get_RepoId (GH_REPOS r) {
    return r->id;
}

char* get_RepoDescription (GH_REPOS r) {
    return strdup(r->description);
}

char * get_updatedDate (GH_REPOS r) {
    struct tm data;
    data.tm_year=r->updated_at.tm_year;
    data.tm_mon=r->updated_at.tm_mon;
    data.tm_mday=r->updated_at.tm_mday;
    char buffer[11];
    strftime(buffer,sizeof(buffer),"%Y-%m-%d", &data); 
    return strdup(buffer);
}

char * get_Creat_at (GH_REPOS r) {
    struct tm data;
    data.tm_year=r->created_at.tm_year;
    data.tm_mon=r->created_at.tm_mon;
    data.tm_mday=r->created_at.tm_mday;
    char buffer[11];
    strftime(buffer,sizeof(buffer),"%Y-%m-%d", &data); 
    return strdup(buffer);
}

C_repos init_repos(C_repos rep){
    rep = malloc(sizeof(C_repos));
    rep -> catalogo_repos = g_hash_table_new_full(g_int_hash,g_int_equal,rmKeyRepos,rmRepos);
    return rep;
}

void free_repos(C_repos rep){
    g_hash_table_remove_all (rep->catalogo_repos);
}

GH_REPOS build_repos(char* line){
    GH_REPOS repos = malloc(sizeof(struct gh_repos));
    char* buffer2 = line;

    /* ID*/    
    buffer2=strdup(strsep(&line, ";\n"));
    repos->id = atoi(buffer2);

    /* OWNER_ID */
    buffer2=strdup(strsep(&line, ";\n"));
    repos->owner_id = atoi(buffer2);

    /* FULL NAME */
    repos->full_name = strdup(strsep(&line,";\n"));

    /* LICENSE */
    repos->license = strdup(strsep(&line,";\n"));

    /* HAS_WIKI*/
    buffer2=strdup(strsep(&line,";\n"));
    if(strcmp(buffer2, "True") == 0) repos->has_wiki = true;
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
    int ano = atoi(strsep(&str_data, "-"));
    repos->created_at.tm_year = ano - 1900;
    int mes = atoi(strsep(&str_data,"-"));
    repos->created_at.tm_mon = mes -1;
    repos->created_at.tm_mday = atoi(strsep(&str_data," "));
    repos->created_at.tm_hour = atoi(strsep(&str_data, ":"));
    repos->created_at.tm_min = atoi(strsep(&str_data, ":"));
    repos->created_at.tm_sec = atoi(strsep(&str_data, ""));

    /* UPDATED_AT*/
    char *str_data2 = strdup(strsep(&line,";\n"));
    int year2, month2, day2, hour2, minutes2, seconds2;
    int nv[6] = { 0 };
    sscanf(str_data2, "%*[0-9]%n-%*[0-9]%n-%*[0-9]%n %*[0-9]%n:%*[0-9]%n:%*[0-9]%n", &nv[0], &nv[1], &nv[2],&nv[3],&nv[4],&nv[5]); 
    if (nv[0] != 4 || nv[1] != 7 || nv[2] != 10 || nv[3] != 13 || nv[4] != 16 || nv[5] != 19){repos->id = -1;}
    sscanf(str_data2, "%d-%d-%d %d:%d:%d", &year2, &month2, &day2, &hour2, &minutes2,&seconds2);
    int ano2 = atoi(strsep(&str_data2, "-"));
    repos->updated_at.tm_year = ano2 - 1900;
    int mes2 = atoi(strsep(&str_data2,"-"));
    repos->updated_at.tm_mon = mes2 -1;
    repos->updated_at.tm_mday = atoi(strsep(&str_data2," "));
    repos->updated_at.tm_hour = atoi(strsep(&str_data2, ":"));
    repos->updated_at.tm_min = atoi(strsep(&str_data2, ":"));
    repos->updated_at.tm_sec = atoi(strsep(&str_data2, ""));

    /* FORKS_COUNT */
    buffer2=strdup(strsep(&line, ";\n"));
    repos->forks_count = atoi(buffer2);

    /* OPEN_ISSUES */
    buffer2=strdup(strsep(&line, ";\n"));
    if(strcmp(buffer2, "")!=0) {
    repos->open_issues = atoi(buffer2);}
    else{repos->open_issues = -1;}

    /* STARGAZERS_COUNT */
    buffer2=strdup(strsep(&line, ";\n"));
    repos->stargazers_count = atoi(buffer2);

    /* SIZE */
    buffer2=strdup(strsep(&line, ";\n"));
    repos->size = atoi(buffer2);
    return repos;
}

//Funçao que dá free a um repositório
void freeRepos(GH_REPOS r){
    free(r->license);
    free(r->description);
    free(r->language);
    free(r->full_name);
    free(r->default_branch);
    free(r);
}

//Função que remove um reposítório da hashTable
void rmRepos(gpointer res){
    GH_REPOS r = res;
    freeRepos(r);
    return;
}

//Função que dá free a uma chave
void rmKeyRepos(gpointer key){
    free((int*)key);
}

void load_catalogo_repos(C_repos r, char * reps){
    r->catalogo_repos = load_repos(reps);
}

GHashTable * load_repos (char *in){
    GH_REPOS rep;
    char buff[LINE_SIZE];
    char ch;
    FILE *f = fopen(in,"r");
    int *key;
    GHashTable * repos  = g_hash_table_new_full(g_int_hash,g_int_equal,rmKeyRepos,rmRepos);

    if(f == NULL){
        perror("Error: ");
        return NULL;
    }

    while((ch = fgetc(f)) != EOF){
        if(ch=='\n') break;
    }
        while(fgets(buff,LINE_SIZE,f)){
            rep = build_repos(buff);
            key = &(rep->id);
            g_hash_table_insert(repos,key,rep);            
        }
    fclose(f);
    //printf("Ficheiro %s lido e verificado\n",in);
    printf("Loading(2/3)\n");
    return repos;
}