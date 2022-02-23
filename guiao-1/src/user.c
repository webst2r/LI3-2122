#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../libs/valid_date.h"
#include "../libs/user.h"

#define LINE_SIZE 1000000

struct gh_user {
    int public_repos;
    int id;
    int followers;
    int public_gists;
    int following;
    int *followers_list;
    int *following_list;
    char *type;
    struct tm created_at;
    char *login;
};

struct catalogo_users {
    GHashTable *table_users;
};


int countOccurrences (char *a,char ch){
    int count = 0;
    int i;
    int length = strlen(a);
    for (i = 0; i < length; i++) {
        if (a[i] == ch) {
            count++;
        }
    }
    return count;
}

char* list_toString(int a[],int b) {
  if(b==0) return "[]";
  int t;int i;
  char *r=malloc(2*sizeof(char));
  strcpy(r,"[");
  char num[10];
  for(i=0;i<b-1;i++){
    sprintf(num, "%d", a[i]);
    t=strlen(num);
    t++;
    r=(char *) realloc(r,strlen(r)+t+2);
    strcat(r,num);
    strcat(r,", ");
    strcpy(num,""); }

  sprintf(num, "%d", a[i]);
  t=strlen(num);
  t++;
  r=(char *) realloc(r,strlen(r)+t+2);
  strcat(r,num);
  strcat(r,"]");
  return r;
}

void print_user (GH_USER u){
    char str_data[20];
    strftime(str_data,sizeof(str_data),"%Y-%m-%d %H:%M:%S",&(u->created_at));
    printf("%d;%s;%s;%s;%d;%s;%d;%s;%d,%d\n",u->id,u->login,u->type,str_data,u->followers,list_toString(u->followers_list, u->followers),u->following,list_toString(u->following_list, u->following) , u->public_gists, u->public_repos);
}

int add_user2file(GH_USER u, FILE *f) {
    char str_data[20];
    strftime(str_data,sizeof(str_data),"%Y-%m-%d %H:%M:%S",&(u->created_at));
    fprintf(f,"%d;%s;%s;%s;%d;%s;%d;%s;%d;%d\n",u->id,u->login,u->type,str_data,u->followers,list_toString(u->followers_list, u->followers),u->following,list_toString(u->following_list, u->following) , u->public_gists, u->public_repos);
    return 0;
}

int userIsValid (GH_USER u) {
    if (u->public_repos<0 || u->id<=0 || u->followers<0 || u->public_gists<0 || u->following<0  ) {return 0;}
    if(strcmp(u->type,"Bot")!=0 && strcmp(u->type,"Organization")!=0 && strcmp(u->type,"User")!=0) {return 0;}
    if(strcmp(u->login,"")==0) {return 0;}

    return 1;
} 

GH_USER build_user(char* line){
    GH_USER u = malloc(sizeof(struct gh_user));
    char* buffer2 = line;

    /* ID */
    buffer2=strdup(strsep(&line, ";\n"));
    if(strcmp(buffer2, "")!=0) {
    u -> id = atoi(buffer2);}
    else{u->id = -1;}

    /* LOGIN */
    u->login = strdup(strsep(&line,";\n"));

    /* TYPE */
    u -> type = strdup(strsep(&line,";\n"));

    /* CREATED_AT */
    char *str_data = strdup(strsep(&line,";\n"));
    int year, month, day, hour, minutes, seconds;
    int nc[6] = { 0 };
    sscanf(str_data, "%*[0-9]%n-%*[0-9]%n-%*[0-9]%n %*[0-9]%n:%*[0-9]%n:%*[0-9]%n", &nc[0], &nc[1], &nc[2],&nc[3],&nc[4],&nc[5]); 
    if (nc[0] != 4 || nc[1] != 7 || nc[2] != 10 || nc[3] != 13 || nc[4] != 16 || nc[5] != 19){u->id = -1;}
    sscanf(str_data, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minutes,&seconds);
    
    if (strlen(str_data) != 19 || !(ValidDateFormat(year, month, day))){u->id = -1;} 
    else {
        int ano = atoi(strsep(&str_data, "-"));
        u->created_at.tm_year = ano - 1900;
        int mes = atoi(strsep(&str_data,"-"));
        u->created_at.tm_mon = mes -1;
        u->created_at.tm_mday = atoi(strsep(&str_data," "));
        u->created_at.tm_hour = atoi(strsep(&str_data, ":"));
        u->created_at.tm_min = atoi(strsep(&str_data, ":"));
        u->created_at.tm_sec = atoi(strsep(&str_data, ""));
    }
    if(!(ValidDate(u->created_at))){u->id = -1;}

    /* FOLLOWERS */
    buffer2=strdup(strsep(&line, ";\n"));
    if(strcmp(buffer2, "")!=0) {
    u -> followers = atoi(buffer2);}
    else{u->followers = -1;}

    /* FOLLOWER_LIST */
    char *Followers = strdup(strsep(&line, ";\n"));
    int n=0;
    if(Followers[1]!=']') {
    n= countOccurrences(Followers,',');
    n++;
    }
    int i=1,j=0,k=0;
    char buff[20]="";
    if(n==u->followers && Followers[0]=='[') {
        u->followers_list=malloc(sizeof(int)*n);
        while(Followers[i]!=']'){
            if(Followers[i]!=',' ) {
                if(Followers[i+1]!=' '){
                    buff[j]=Followers[i];
                    j++;}
                else i++;}
            else {
              j=0;
              u->followers_list[k]=atoi(buff);
              if(u->followers_list[k]<0) {
                  u->followers=-1;
                  break;
              }
              k++;
              }  
              i++;
        }
    u->followers_list[k]=atoi(buff);
    }
    else {u->followers=-1;}

    /* FOLLOWING */
    buffer2=strdup(strsep(&line, ";\n"));
    if(strcmp(buffer2, "")!=0) {
    u->following = atoi(buffer2);}
    else{u->following = -1;}

    /* FOLLOWING_LIST */
    char *Following = strdup(strsep(&line, ";\n"));
    n=0;
    if(Following[1]!=']') {
    n= countOccurrences(Following,',');
    n++;
    }
    if(n==u->following && Following[0]=='[') {
        i=1;j=0;k=0;
        u->following_list=malloc(sizeof(int)*n);
        while(Following[i]!=']'){
            if(Following[i]!=',' ) {
                if(Following[i+1]!=' '){
                    buff[j]=Following[i];
                    j++;}
                else {i++;}
                }
            else {
              j=0;
              u->following_list[k]=atoi(buff);
              if(u->following_list[k]<0) {
                  u->following=-1;
                  break;
              }
              k++;
              }  
              i++;
        }
    u->following_list[k]=atoi(buff);
    }
    else {u->following=-1;}

    /* PUBLIC_GISTS */
    buffer2=strdup(strsep(&line, ";\n"));
    if(strcmp(buffer2, "")!=0) {
    u -> public_gists = atoi(buffer2);}
    else{u->public_gists = -1;}

    /* PUBLIC_REPOS */
    buffer2=strdup(strsep(&line, ";\n"));
    if(strcmp(buffer2, "")!=0) {
    u -> public_repos = atoi(buffer2);}
    else{u->public_repos = -1;}
    
    return u;
}
//Funçao que dá free a um user

void freeUser(GH_USER u){
    free((u->login));
    free((u->type));
    free((u->followers_list));
    free((u->following_list));
    free(u);
}

//Função que remove um user da hashTable
void rmUser(gpointer usr){
    GH_USER u = usr;
    freeUser(u);
    return;
}

//Função que dá free a uma chave
void rmKeyUser(gpointer key){
    free((int*)key);
}

GHashTable *load_users (char *in, char *out){
    GH_USER u;
    char buff[LINE_SIZE];
    char ch;
    FILE *f = fopen(in,"r");
    FILE *g = fopen(out,"w");
    int *key;
    int validos = 0;
    GHashTable * users  = g_hash_table_new_full(g_int_hash,g_int_equal,rmKeyUser,rmUser);
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
            u=build_user(buff);
            if(userIsValid(u)) {
                add_user2file(u,g);
                key = &(u->id);
                g_hash_table_insert(users,key,u);
                validos++;
            }
        }
    fclose(f);
    fclose(g);
    printf("Ficheiro lido e verificado\n");
    printf("Registos Validos em %s: %d\n\n",in,validos);//

    return users;
}
