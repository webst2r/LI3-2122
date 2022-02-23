#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/user.h"

#define LINE_SIZE 1000000
int bot=0;
int organization=0;
int user=0;

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

struct c_users {
    GHashTable *catalogo_us;
};

int getUserId(GH_USER u){
    return u->id;
}

int *getFollowersList(GH_USER u){
    return u->followers_list;
}

int getFollowers(GH_USER u){
    return u->followers;
}

int *getFollowingList(GH_USER u){
    return u->following_list;
}

int getFollowing(GH_USER u){
    return u->following;
}

int get_Size_User (C_users c) {
    return g_hash_table_size(c->catalogo_us);
}

GHashTable * get_Table_User(C_users c) {return c->catalogo_us; }

int get_bot() {return bot;}

int get_organization() {return organization;}

int get_user() {return user;}

char* getLogin(GH_USER u) {
    return strdup(u->login);
}

C_users init_users(C_users us){
    us = malloc(sizeof(C_users));
    us -> catalogo_us = g_hash_table_new_full(g_int_hash,g_int_equal,rmKeyUser,rmUser);
    return us;
}

GHashTable * get_catalogoUsers(C_users cusers) {
    return (cusers->catalogo_us);
}

void free_users(C_users us){
    g_hash_table_remove_all (us->catalogo_us);
}

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

GH_USER build_user(char* line){
    GH_USER u = malloc(sizeof(struct gh_user));
    char* buffer2 = line;

    /* ID */
    buffer2=strdup(strsep(&line, ";\n"));
    u -> id = atoi(buffer2);

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
    int ano = atoi(strsep(&str_data, "-"));
    u->created_at.tm_year = ano - 1900;
    int mes = atoi(strsep(&str_data,"-"));
    u->created_at.tm_mon = mes -1;
    u->created_at.tm_mday = atoi(strsep(&str_data," "));
    u->created_at.tm_hour = atoi(strsep(&str_data, ":"));
    u->created_at.tm_min = atoi(strsep(&str_data, ":"));
    u->created_at.tm_sec = atoi(strsep(&str_data, ""));

    /* FOLLOWERS */
    buffer2=strdup(strsep(&line, ";\n"));
    u -> followers = atoi(buffer2);

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
    u->following = atoi(buffer2);

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
    u -> public_gists = atoi(buffer2);

    /* PUBLIC_REPOS */
    buffer2=strdup(strsep(&line, ";\n"));
    u -> public_repos = atoi(buffer2);
    
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

void load_catalogo_users(C_users us, char * users){
    us->catalogo_us = load_users(users);
}

GHashTable *load_users (char *in){
    GH_USER u;
    char buff[LINE_SIZE];
    char ch;
    FILE *f = fopen(in,"r");
    int *key;
    GHashTable * users  = g_hash_table_new_full(g_int_hash,g_int_equal,rmKeyUser,rmUser);

    if(f == NULL){
        perror("Error: ");
        return NULL;
    }

    while((ch = fgetc(f)) != EOF){
        if(ch=='\n') break;
    }
    
        while(fgets(buff,LINE_SIZE,f)){
            u = build_user(buff);
            if(strcmp(u->type,"Bot")==0) bot++;
            else if(strcmp(u->type,"Organization")==0)organization++;
            else if(strcmp(u->type,"User")==0) user++;
            key = &(u->id);
            g_hash_table_insert(users,key,u);
        }
    fclose(f);
    //printf("Ficheiro %s lido e verificado\n",in);    
    printf("Loading(1/3)\n");
    return users;
}