#include "../include/user.h"
#include "../include/commits.h"
#include "../include/repos.h"
#include "../include/valid_date.h"
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

GH_REPOS lookupRepo(C_repos repos, int *id){
    GH_REPOS r = g_hash_table_lookup(repos->catalogo_repos,id);
    GH_REPOS repo = malloc(sizeof(struct gh_repos));
    repo->default_branch = strdup(r->default_branch);
    repo->description = strdup(r->description);
    repo->forks_count = r->forks_count;
    repo->full_name = strdup(r->full_name);
    repo->created_at = r->created_at;
    repo->has_wiki = r->has_wiki;
    repo->id = r->id;
    repo->language = strdup(r->language);
    repo->license = strdup(r->license);
    repo->open_issues = r->open_issues;
    repo->owner_id = r->owner_id;
    repo->size = r->size;

    return repo;
}

gboolean tableContainsRepo(C_repos repos,int id){
    return g_hash_table_contains(repos->catalogo_repos,&id);
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

C_repos init_repos(){
    C_repos rep = malloc(sizeof(C_repos));
    rep -> catalogo_repos = g_hash_table_new(g_int_hash,g_int_equal);
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

int reposIsValid (GH_REPOS rep) {
    if (rep->forks_count<0 || rep->open_issues<0 || rep->stargazers_count<0 || rep->owner_id<=0 || rep->id<=0 || rep->size<0) {return 0;}
    if(strcmp(rep->license,"")==0 || strcmp(rep->language,"")==0 || strcmp(rep->full_name,"")==0 || strcmp(rep->default_branch,"")==0) {return 0;}
    return 1;
}

GHashTable * load_repos (char *in){
    GH_REPOS rep;
    char buff[LINE_SIZE];
    char ch;
    FILE *f = fopen(in,"r");
    int *key;
    GHashTable * repos  = g_hash_table_new(g_int_hash,g_int_equal);


    if(f == NULL){
        perror("Error: ");
        return NULL;
    }

    while((ch = fgetc(f)) != EOF){
        if(ch=='\n') break;
    }
        while(fgets(buff,LINE_SIZE,f)){
            rep = build_repos(buff);
            if(reposIsValid(rep)){
                key = &(rep->id);
                g_hash_table_insert(repos,key,rep); 
            }           
        }
    fclose(f);
    //printf("Ficheiro %s lido e verificado\n",in);
    printf("                     \033[1;31m║\033[0m ( 2/3) \033[0;35mloading repos\033[0m         [##################################################] 100%%\033[1;31m     ║\n");
    return repos;
}


// QUERY8
struct valor {
    int num;
};

struct linguagem {
    char *linguagem;
    int ocorrencias;
};

struct dadosQuery8 { 
    char *data_inicio;
    int NU;
    GHashTable *counts;
    LINGUAGEM *l;
};


void calculaUtilizacao (gpointer key, gpointer value, gpointer userData) {
    DADOSQUERY8 dados=(DADOSQUERY8) userData;
    GH_REPOS repo = (GH_REPOS) value;
    char date[12];
    char *date2 = strdup(dados->data_inicio);
    char *chave;
    VALOR valor;
    strcpy(date,get_updatedDate(repo));
    
    if(posterior(date,date2)==0) {
        chave = getReposLanguage(repo);
        valor= g_hash_table_lookup(dados->counts,chave);
        if( valor == NULL){
            VALOR new = malloc(sizeof(struct valor));
            new->num=1;
            g_hash_table_insert(dados->counts,chave,new);
        } else {
            valor->num=valor->num+1;
            g_hash_table_insert(dados->counts,chave,valor);
        }
    } 
}

void getTopLinguagensUtilizadas (gpointer key, gpointer value, gpointer userData) {
    DADOSQUERY8 dados=(DADOSQUERY8) userData;
    VALOR v = (VALOR) value;
    int i;
    for(i=0; i<(dados->NU) && dados->l[i]->ocorrencias > v->num;i++) {}
    if(i!=dados->NU) {
        for(int j=(dados->NU)-1;j>i;j--) {
            dados->l[j]->ocorrencias=dados->l[j-1]->ocorrencias;
            dados->l[j]->linguagem=dados->l[j-1]->linguagem;
        }
        dados->l[i]->ocorrencias=v->num;
        dados->l[i]->linguagem=strdup(key);
    }
}

GArray* determinaLinguagensMaisUtilizadas(C_repos repos,int N,char *dataInicio,FILE *f, int tipo) {
    int i;
    DADOSQUERY8 dados = malloc(sizeof(struct dadosQuery8));
    dados->NU =N;
    dados->data_inicio = dataInicio;
    dados->counts = g_hash_table_new(g_str_hash,g_str_equal);
    dados->l = malloc(N * sizeof(struct linguagem)); 
    
    for(i=0;i<N;i++) {
        dados->l[i]=malloc(sizeof(struct linguagem));
        dados->l[i]->ocorrencias=-1;
        dados->l[i]->linguagem="";
    }
    g_hash_table_foreach(repos->catalogo_repos,calculaUtilizacao,dados);
    g_hash_table_foreach(dados->counts,getTopLinguagensUtilizadas,dados); 
    
    if(tipo == 0){
        // ficheiro
        for(i=0;i<N;i++) {
            fprintf(f,"%s; %d\n",dados->l[i]->linguagem,dados->l[i]->ocorrencias);
        }
        return NULL;
    } else {
        GArray* arrayStrings = g_array_new(FALSE, FALSE, sizeof(char*));
        for(i=0;i<N;i++) {
            char str[50];
            sprintf(str,"| %s | %d |",dados->l[i]->linguagem,dados->l[i]->ocorrencias);
            char* aux = strdup(str);
            g_array_append_val(arrayStrings, aux);
        }
        return arrayStrings;
    }
}

// VALIDATE & START

struct validaRepos{
    C_users users;
    C_commits commits;
};



gboolean filtraRepo(gpointer key, gpointer value, gpointer userdata){
    VALIDAREPOS dados = (VALIDAREPOS) userdata;

    GH_REPOS rep = (GH_REPOS) value;
    int owner_id = get_OwnerId(rep);
    int *id = (int*) key;

    if(tableContainsCommit(dados->commits,*id) && tableContainsUser(dados->users,owner_id)){
        GArray *commitsRepo = lookupCommit(dados->commits,id);
        if(commitsRepo->len == 0){
            return TRUE;
        } else return FALSE;
    } else return TRUE;
}


void filtraRepos(C_repos repos,C_commits coms,C_users users){
    VALIDAREPOS dados=malloc(sizeof(struct validaRepos));
    dados->users = users;
    dados->commits = coms;
    g_hash_table_foreach_steal(repos->catalogo_repos,filtraRepo,dados);
}
