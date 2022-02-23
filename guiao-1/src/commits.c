#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../libs/valid_date.h"
#include "../libs/commits.h"

#define LINE_SIZE 1000000

struct gh_commit {
    int repo_id;
    int author_id;
    int committer_id;
    struct tm commit_at;
    char *message;
};

int get_repo_id(GH_COMMIT com) {return com->repo_id;}

int get_author_id(GH_COMMIT com) {return com->author_id;}

int get_committer_id(GH_COMMIT com) {return com->committer_id;}

int commitIsValid (GH_COMMIT gc) {
    if (gc->repo_id<=0|| gc->committer_id<=0 || gc->author_id<=0) {return 0;}   
    return 1;
}

int add_commit2file(GH_COMMIT gc, FILE *f) {
    // repo_id;author_id;committer_id;commit_at;message
    char str_data[20];
    strftime(str_data,sizeof(str_data),"%Y-%m-%d %H:%M:%S",&(gc->commit_at));
    fprintf(f,"%d;%d;%d;%s;%s\n",gc->repo_id, gc->author_id, gc->committer_id,str_data,gc->message);
    return 0;
}

void print_commit (GH_COMMIT gc){
    char str_data[20];
    strftime(str_data,sizeof(str_data),"%Y-%m-%d %H:%M:%S",&(gc->commit_at));
    printf("%d;%d;%d;%s;%s\n",gc->repo_id, gc->author_id, gc->committer_id,str_data,gc->message);
}


GH_COMMIT build_commit(char* line){
    GH_COMMIT commit = malloc(sizeof(struct gh_commit));

    // REPO_ID    
    char* buffer2 = line;
    buffer2=strdup(strsep(&line, ";\n"));
    if(strcmp(buffer2, "")!=0) {
    commit->repo_id = atoi(buffer2);}
    else{commit->repo_id = -1;}

    // AUTHOR_ID 
    buffer2=strdup(strsep(&line, ";\n"));
    if(strcmp(buffer2, "")!=0) {
    commit->author_id = atoi(buffer2);}
    else{commit->author_id = -1;}

    // COMMITER_ID 
    buffer2=strdup(strsep(&line, ";\n"));
    if(strcmp(buffer2, "")!=0) {
    commit->committer_id = atoi(buffer2);}
    else{commit->committer_id = -1;}


    // COMMIT_AT 
    char *str_data = strdup(strsep(&line,";\n"));
    int year, month, day, hour, minutes, seconds;
    int nc[6] = { 0 };
    sscanf(str_data, "%*[0-9]%n-%*[0-9]%n-%*[0-9]%n %*[0-9]%n:%*[0-9]%n:%*[0-9]%n", &nc[0], &nc[1], &nc[2],&nc[3],&nc[4],&nc[5]); 
    if (nc[0] != 4 || nc[1] != 7 || nc[2] != 10 || nc[3] != 13 || nc[4] != 16 || nc[5] != 19){commit->author_id = -1;}
    sscanf(str_data, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minutes,&seconds);
    
    if (strlen(str_data) != 19 || !(ValidDateFormat(year, month, day))) {commit->author_id = -1;} 
    else {
        int ano = atoi(strsep(&str_data, "-"));
        commit->commit_at.tm_year = ano - 1900;
        int mes = atoi(strsep(&str_data,"-"));
        commit->commit_at.tm_mon = mes -1;
        commit->commit_at.tm_mday = atoi(strsep(&str_data," "));
        commit->commit_at.tm_hour = atoi(strsep(&str_data, ":"));
        commit->commit_at.tm_min = atoi(strsep(&str_data, ":"));
        commit->commit_at.tm_sec = atoi(strsep(&str_data, ""));
    }
    if(!(ValidDate(commit->commit_at))){commit->author_id = -1;}
    
    commit->message = strdup(strsep(&line,";\n"));

    return commit;
}

GArray * load_commits (char *in, char *out){
    GH_COMMIT com;
    char ch;
    char buff[LINE_SIZE];
    int validos = 0;
    GArray * commits  = g_array_new(FALSE,FALSE,sizeof(GH_COMMIT));
    FILE *f = fopen(in,"r");
    FILE *g = fopen(out,"w");
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
            com = build_commit(buff);
            if(commitIsValid(com)) {
                validos++;
                add_commit2file(com,g);
                commits=g_array_append_val(commits,com);
           }
        }
    fclose(f);
    fclose(g);
    printf("Ficheiro lido e verificado\n");
    printf("Registos Validos em %s: %d\n\n",in,validos);
    return commits;
}
