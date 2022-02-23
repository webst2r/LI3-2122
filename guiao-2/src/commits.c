#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/commits.h"
#include "../include/repos.h"
#include "../include/user.h"
int nColaboradores=0;
int repos_with_bots = 0;
int nCommits=0;

#define LINE_SIZE 1000000

struct gh_commit {
    int repo_id;
    int author_id;
    int committer_id;
    struct tm commit_at;
    char *message;
};

int get_NColaboradores() {return nColaboradores;}

int get_Repos_with_Bots() {return repos_with_bots;}

int get_Committer_Id (GH_COMMIT c) {
    return c->committer_id;
}

char *getCommitMessage(GH_COMMIT c){
    return strdup(c->message);
}

char * get_commit_data(GH_COMMIT c) {
    struct tm data;
    data.tm_year=c->commit_at.tm_year;
    data.tm_mon=c->commit_at.tm_mon;
    data.tm_mday=c->commit_at.tm_mday;
    char buffer[11];
    strftime(buffer,sizeof(buffer),"%Y-%m-%d", &data); 
    return strdup(buffer);
}

struct c_commits {
    GHashTable *catalogo_commits;
};

GHashTable * get_catalogoCommits(C_commits cComs) {
    return (cComs->catalogo_commits);
}

int get_nCommits (C_commits c) {
    return nCommits;
}

C_commits init_commits(C_commits coms){
    coms = malloc(sizeof(C_commits));
    coms -> catalogo_commits =g_hash_table_new(g_int_hash,g_int_equal);
    return coms;
}

void free_commits(C_commits coms){
    g_hash_table_remove_all (coms->catalogo_commits);
}

GH_COMMIT build_commit(char* line){
    GH_COMMIT commit = malloc(sizeof(struct gh_commit));

    // REPO_ID    
    char* buffer2 = line;
    buffer2=strdup(strsep(&line, ";\n"));
    commit->repo_id = atoi(buffer2);

    // AUTHOR_ID 
    buffer2=strdup(strsep(&line, ";\n"));
    commit->author_id = atoi(buffer2);

    // COMMITER_ID 
    buffer2=strdup(strsep(&line, ";\n"));
    commit->committer_id = atoi(buffer2);

    // COMMIT_AT 
    char *str_data = strdup(strsep(&line,";\n"));
    int year, month, day, hour, minutes, seconds;
    int nc[6] = { 0 };
    sscanf(str_data, "%*[0-9]%n-%*[0-9]%n-%*[0-9]%n %*[0-9]%n:%*[0-9]%n:%*[0-9]%n", &nc[0], &nc[1], &nc[2],&nc[3],&nc[4],&nc[5]); 
    if (nc[0] != 4 || nc[1] != 7 || nc[2] != 10 || nc[3] != 13 || nc[4] != 16 || nc[5] != 19){commit->author_id = -1;}
    sscanf(str_data, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minutes,&seconds);

    int ano = atoi(strsep(&str_data, "-"));
    commit->commit_at.tm_year = ano - 1900;
    int mes = atoi(strsep(&str_data,"-"));
    commit->commit_at.tm_mon = mes -1;
    commit->commit_at.tm_mday = atoi(strsep(&str_data," "));
    commit->commit_at.tm_hour = atoi(strsep(&str_data, ":"));
    commit->commit_at.tm_min = atoi(strsep(&str_data, ":"));
    commit->commit_at.tm_sec = atoi(strsep(&str_data, ""));
    
    commit->message = strdup(strsep(&line,";\n"));
    
    return commit;
}

void load_catalogo_commits(C_commits c, char * coms,GHashTable *h){
    c->catalogo_commits = load_commits(coms,h);
}

GHashTable * load_commits (char *in,GHashTable *c){
    GH_COMMIT com;
    char ch;
    char buff[LINE_SIZE];
    int *keyRepo;
    int *key1;
    int *key2;
    GHashTable * commits  = g_hash_table_new(g_int_hash,g_int_equal);
    GHashTable * repos_bots  = g_hash_table_new(g_int_hash,g_int_equal);
    GHashTable * colaboradores = g_hash_table_new(g_int_hash,g_int_equal);
    
    FILE *f = fopen(in,"r");
    if(f == NULL){
        perror("Error: ");
        return NULL;
    }

    while((ch = fgetc(f)) != EOF){
        if(ch=='\n') break;
    }
    
        while(fgets(buff,LINE_SIZE,f)){
            com = build_commit(buff);
            nCommits++;
            key1 = &(com->author_id); 
            g_hash_table_insert(colaboradores,key1,key1);
            key2 = &(com->committer_id); 
            g_hash_table_insert(colaboradores,key2,key2);
            keyRepo=&(com->repo_id);

            if(g_hash_table_contains(c,key1) || g_hash_table_contains(c,key2) ) {
                g_hash_table_insert(repos_bots,keyRepo,keyRepo);
            }
            
            if(g_hash_table_contains(commits,keyRepo)!=0) {
                GArray * values=g_hash_table_lookup(commits,keyRepo);
                g_array_append_val(values,com);
                g_hash_table_insert(commits,keyRepo,values);
            }
            else {
                GArray * valuesEmpty  = g_array_new(FALSE,FALSE,sizeof(GH_COMMIT));
                g_array_append_val(valuesEmpty,com);
                g_hash_table_insert(commits,keyRepo,valuesEmpty);
            }
           }
    repos_with_bots=g_hash_table_size(repos_bots);
    nColaboradores=g_hash_table_size(colaboradores);
    fclose(f);
    //printf("Ficheiro %s lido e verificado\n\n",in);
    printf("Loading(3/3)\n");
    return commits;
}