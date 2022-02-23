#include "../include/commits.h"
#include "../include/valid_date.h"
#include "../include/sgg.h"

int nColaboradores=0;
int repos_with_bots = 0;
int nCommits=0;

#define LINE_SIZE 1000000

struct gh_commit {
    int repo_id;
    int author_id;
    int committer_id;
    struct tm commit_at;
    int message;
};

int get_NColaboradores() {return nColaboradores;}

int get_Repos_with_Bots() {return repos_with_bots;}

int get_Committer_Id (GH_COMMIT c) { return c->committer_id;}

int get_author_id(GH_COMMIT com) { return com->author_id;}

int get_repo_id(GH_COMMIT com) { return com->repo_id;}

int getCommitMessage(GH_COMMIT c){ return c->message;}

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


int get_nCommits (C_commits c) {
    return nCommits;
}

C_commits init_commits(){
    C_commits coms = malloc(sizeof(C_commits));
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
    
    commit->message = strlen(strsep(&line,";\n"));
    
    return commit;
}

void load_catalogo_commits(C_commits c, char * coms,C_users us){
    c->catalogo_commits = load_commits(coms,us);
}


GArray* lookupCommit(C_commits coms, int *id){
    GArray* commits= g_hash_table_lookup(coms->catalogo_commits,id);
    GArray* copy = g_array_new(FALSE,FALSE,sizeof(GH_COMMIT));
    for(int i = 0; i < commits->len; i++){
        GH_COMMIT com = g_array_index(commits,GH_COMMIT,i);
        GH_COMMIT commit = malloc(sizeof(struct gh_commit));
        commit->author_id = com->author_id;
        commit->commit_at = com->commit_at;
        commit->committer_id = com->committer_id;
        commit->message = com->message;
        commit->repo_id = com->repo_id;
        g_array_append_val(copy,commit);
    }
    return copy;
}

gboolean tableContainsCommit(C_commits coms,int id){
    return g_hash_table_contains(coms->catalogo_commits,&id);
}


int commitIsValid (GH_COMMIT gc) {
    if (gc->repo_id<=0|| gc->committer_id<=0 || gc->author_id<=0) {return 0;}   
    return 1;
}


GHashTable * load_commits (char *in,C_users us){
    GH_COMMIT com;
    char ch;
    char buff[LINE_SIZE];
    int *keyRepo;
    int key1,key2;
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
            if(commitIsValid(com)){
                nCommits++;
                key1 = com->author_id; 
                g_hash_table_insert(colaboradores,&key1,&key1);
                key2 = com->committer_id; 
                g_hash_table_insert(colaboradores,&key2,&key2);
                keyRepo=&(com->repo_id);

                if(tableContainsUser(us,key1) || tableContainsUser(us,key2) ) {
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
        }
    repos_with_bots=g_hash_table_size(repos_bots);
    nColaboradores=g_hash_table_size(colaboradores);
    fclose(f);
    //printf("Ficheiro %s lido e verificado\n\n",in);
    printf("                     \033[1;31m║\033[0m ( 3/3) \033[0;35mloading commits\033[0m       [##################################################] 100%%\033[1;31m     ║\n");
    return commits;
}



// QUERY5 

struct value {
    int num;
};

struct atividade {
    int id;
    int ocorrencias;
};

struct dadosQuery5 {
    char *data_inicio;
    char *data_fim;
    int NU;
    GHashTable *counts;
    ATIVIDADE *a;
};

void getTopMaisAtivos (gpointer key, gpointer value, gpointer userData) {
    DADOSQUERY5 dados=(DADOSQUERY5) userData;
    VALUE v = (VALUE) value;
    int i;
    for(i=0; i<(dados->NU) && dados->a[i]->ocorrencias > v->num;i++) {}
    if(i!=dados->NU) {
        for(int j=(dados->NU)-1;j>i;j--) {
            dados->a[j]->ocorrencias=dados->a[j-1]->ocorrencias;
            dados->a[j]->id=dados->a[j-1]->id;
        }
        dados->a[i]->ocorrencias=v->num;
        int *id=key;
        dados->a[i]->id=(*id);
    }
}

void calculaAtividade (gpointer key, gpointer value, gpointer userData) {
    DADOSQUERY5 dados=(DADOSQUERY5) userData;
    GArray *commits= value;
    GH_COMMIT com;
    VALUE valor;
    char date[12];   
    char date2[12]; 
    for(int i=0;i<commits->len;i++) { 
        com=g_array_index(commits,GH_COMMIT,i);
        strcpy(date,get_commit_data(com));
        strcpy(date2,date);
        char inicio[12], fim[12];
        strcpy(inicio,dados->data_inicio);
        strcpy(fim,dados->data_fim);
        if(posterior(date,inicio)==1 && posterior(date2,fim)==0) {
            int chave= get_Committer_Id(com);
            valor = g_hash_table_lookup(dados->counts,&chave);
            if(valor==NULL) {
                int *c = malloc(sizeof(int));
                (*c)= get_Committer_Id(com);
                VALUE new = malloc(sizeof(struct value));
                new->num=1;
                g_hash_table_insert(dados->counts,c,new);
            } 
            else {
                int *d = malloc(sizeof(int));
                (*d)= get_Committer_Id(com);
                valor->num=valor->num+1;
                g_hash_table_insert(dados->counts,d,valor);
            }
        } 
    }
    
}



GArray* calculaUtilizadoresMaisAtivos(C_commits coms,C_users users,int N,char *dataInicio,char *dataFim,FILE *f, int tipo) {
    DADOSQUERY5 dados=malloc(sizeof(struct dadosQuery5));
    dados->data_fim =dataFim;
    dados->data_inicio = dataInicio;
    dados->NU=N;
    dados->counts = g_hash_table_new(g_int_hash,g_int_equal);
    dados->a = malloc(N * sizeof(struct atividade)); 
    int i;
    for(i=0;i<N;i++) {
        dados->a[i]=malloc(sizeof(struct atividade));
        dados->a[i]->ocorrencias=-1;
        dados->a[i]->id=-1;
    }

    
    g_hash_table_foreach(coms->catalogo_commits,calculaAtividade,dados);
    g_hash_table_foreach(dados->counts,getTopMaisAtivos,dados); 
    
    if(tipo == 0){
        // ficheiro
        for(i=0;i<N;i++) {
        int key=dados->a[i]->id;
        GH_USER u = lookupUser(users,key);
        char *login = strdup(getLogin(u));
        fprintf(f,"%d; %s; %d\n",key,login,dados->a[i]->ocorrencias);
        }
        return NULL;
    } else {
        GArray* arrayStrings = g_array_new(FALSE, FALSE, sizeof(char*)); 
        for(i=0;i<N;i++) {
            int key=dados->a[i]->id;
            GH_USER u = lookupUser(users,key);
            char *login = strdup(getLogin(u));
            char str[50];
            sprintf(str,"| %d | %s | %d |",key,login,dados->a[i]->ocorrencias);
            char* aux = strdup(str);
            g_array_append_val(arrayStrings,aux);
        }
    
    return arrayStrings;
}
}



// QUERY6
struct committer {
    int idCommitter;
    int count;
};

struct dadosQuery6 {
    int NU;
    char *linguagem;
    GHashTable *counts;
    C_repos repos;
    COMMITTER *c;
};

void getTopUtilizadoresLinguagem (gpointer key, gpointer value, gpointer userData) {
    DADOSQUERY6 dados=(DADOSQUERY6) userData;
    VALUE v = (VALUE) value;
    int i;
    for(i=0; i < (dados->NU) && dados->c[i]->count > v->num;i++) {}
    
    if(i!=dados->NU) {
        for(int j=(dados->NU)-1;j>i;j--){
            dados->c[j]->count = dados->c[j-1]->count;
            dados->c[j]->idCommitter = dados->c[j-1]->idCommitter;
        }
        dados->c[i]->count=v->num;
        int *p = (int*)key;
        dados->c[i]->idCommitter = *p;
    }
}


void calculaCommitsLinguagem (gpointer key, gpointer value, gpointer userData) {
    DADOSQUERY6 dados = (DADOSQUERY6) userData;
    GArray *arrComs = (GArray *) value;
    GH_COMMIT com;
    GH_REPOS repo;
    int committerId;
    VALUE valor;

    repo = lookupRepo(dados->repos,(int*)key);
    if(repo != NULL){
        char *lang = strdup(toLower(getReposLanguage(repo)));
        if(strcmp(dados->linguagem, lang) == 0){
            for(int i = 0; i < arrComs->len; i++){
                com = g_array_index(arrComs,GH_COMMIT,i);
                committerId = get_Committer_Id(com);
                valor = g_hash_table_lookup(dados->counts,&committerId);

                if(valor == NULL){
                    int *c = malloc(sizeof(int));
                    (*c)= get_Committer_Id(com);
                    VALUE new = malloc(sizeof(struct value));
                    new->num=1;
                    g_hash_table_insert(dados->counts,c,new);
                } else {
                    int *d = malloc(sizeof(int));
                    (*d)= get_Committer_Id(com);
                    valor->num=valor->num+1;
                    g_hash_table_insert(dados->counts,d,valor);
                }
            }
        }
    }
}

GArray* calculaUtilizadoresMaisCommitsLinguagem(C_commits coms,C_users users,C_repos repos,int N,char *language, FILE *f, int tipo) {
    DADOSQUERY6 dados=malloc(sizeof(struct dadosQuery6));
    dados->NU = N;
    dados->linguagem = language;
    dados->counts = g_hash_table_new(g_int_hash,g_int_equal);
    dados->repos = repos;
    dados->c = malloc(N * sizeof(struct committer)); 
    int i;
    for(i=0;i<N;i++) {
        dados->c[i]= malloc(sizeof(struct committer));
        dados->c[i]->count = -1;
        dados->c[i]->idCommitter = 0;
    }

    g_hash_table_foreach(coms->catalogo_commits,calculaCommitsLinguagem,dados);
    g_hash_table_foreach(dados->counts,getTopUtilizadoresLinguagem, dados); 

    if(tipo == 0){
        // ficheiro
        for(i=0;i<N;i++) {
        int key = dados->c[i]->idCommitter;
        GH_USER u = lookupUser(users, key);
        char *login = strdup(getLogin(u));
        fprintf(f,"%d;%s;%d\n",dados->c[i]->idCommitter,login,dados->c[i]->count);
        }  
        return NULL;
    } else {
        GArray* arrayStrings = g_array_new(FALSE, FALSE, sizeof(char*));
        for(i=0;i<N;i++) {
        int key = dados->c[i]->idCommitter;
        GH_USER u = lookupUser(users, key);
        char *login = strdup(getLogin(u));
        char str[50];
        sprintf(str,"| %d | %s | %d |",dados->c[i]->idCommitter,login,dados->c[i]->count);
        char* aux = strdup(str);
        g_array_append_val(arrayStrings, aux);
        }
        return arrayStrings;  
    }
}

// QUERY 7
struct dadosQuery7 { 
    char *data_inicio;
    FILE *out;
    C_repos repos;
    GArray* arrayStrs;
    int tipo;
};

void filtraInativos (gpointer key, gpointer value, gpointer userData) {
    DADOSQUERY7 dados=(DADOSQUERY7) userData;
    GArray *arrComs = (GArray *) value;
    char date[12];
    char date2[12];
    int r=0;
    GH_COMMIT com;
    for(int i = 0; i < arrComs->len && r!=1; i++){ 
        com= g_array_index(arrComs,GH_COMMIT,i);
        strcpy(date,get_commit_data(com));
        strcpy(date2,dados->data_inicio);
        if(posterior(date,date2)==1) {
                r=1;
        }}
        if(dados->tipo == 0) {  
            if(r==0) {
                GH_REPOS repositorio = lookupRepo(dados->repos,(int*)key); 
                char* des = get_RepoDescription (repositorio);
                int *id=key;
                fprintf(dados->out,"%d;%s\n",(*id),des);
            } 
        } else {
            if(r==0) {
                GH_REPOS repositorio = lookupRepo(dados->repos,(int*)key);  
                char* des = get_RepoDescription (repositorio), *aux;
                int *id=key;
                char str[100000];
                sprintf(str,"| %d | %s |",(*id),des);
                aux = strdup(str);
                g_array_append_val(dados->arrayStrs, aux);
            }
        }
}

GArray* determinaReposInativos(C_commits coms, C_repos repos, char *dataInicio,FILE *f, int tipo) {
    DADOSQUERY7 dados = malloc(sizeof(struct dadosQuery7));
    dados ->data_inicio=dataInicio;
    dados->out =f;
    dados->repos = repos;
    dados->arrayStrs = g_array_new(FALSE, FALSE, sizeof(char*));
    dados->tipo = tipo;
    g_hash_table_foreach(coms->catalogo_commits,filtraInativos,dados);

    return dados->arrayStrs;
}

// QUERY9
struct user {
    int id;
    int count;
};

struct dadosQuery9 {
    int NU;
    GHashTable *counts;
    C_users users;
    C_repos repos;
    USER *u;
};
void calculaCommitsAmigoOwner (gpointer key, gpointer value, gpointer userData) {
    DADOSQUERY9 dados = (DADOSQUERY9) userData;
    GArray *arrComs = (GArray *) value;
    GH_COMMIT com;
    int committerID, ownerID;
    GH_REPOS repo;
    VALUE valor;

    for(int i = 0; i < arrComs->len; i++) {
        com = g_array_index(arrComs, GH_COMMIT, i);
        repo = lookupRepo(dados->repos, key);

        if(repo != NULL) {
            ownerID = get_OwnerId(repo);
            committerID = get_Committer_Id(com);
            GH_USER owner = lookupUser(dados->users, ownerID);
            GH_USER committer = lookupUser(dados->users, committerID);
            
            if(isFriend(owner, committer) != 0) {
                valor = g_hash_table_lookup(dados->counts, &committerID);
                if(valor == NULL){
                    int *c = malloc(sizeof(int));
                    (*c)= get_Committer_Id(com);
                    VALUE new = malloc(sizeof(struct value));
                    new->num=1;
                    g_hash_table_insert(dados->counts,c, new);
                } else {
                    int *d = malloc(sizeof(int));
                    (*d)= get_Committer_Id(com);
                    valor->num=valor->num+1;
                    g_hash_table_insert(dados->counts,d, valor);
                }
            }
        }
    }
}



void getTopUsersAmigoOwner(gpointer key, gpointer value, gpointer userData) {
    DADOSQUERY9 dados= (DADOSQUERY9) userData;
    VALUE v = (VALUE) value;
    int i;
    for(i = 0; i < (dados->NU) && dados->u[i]->count > v->num; i++) {}
        
        if(i != dados->NU) {
            for(int j = (dados->NU) - 1; j > i; j--) {
                dados->u[j]->count = dados->u[j-1]->count;
                dados->u[j]->id = dados->u[j-1]->id;
            }
            dados->u[i]->count=v->num;
            int *p = (int*) key;
            dados->u[i]->id = *p;
        }
}

GArray* determinaTopUsersAmigoOwner(C_commits coms,C_repos repos,C_users users,int N,FILE *f, int tipo) {
    DADOSQUERY9 dados = malloc(sizeof(struct dadosQuery9));
    dados->NU = N;
    dados->counts = g_hash_table_new(g_int_hash,g_int_equal);
    dados->repos = repos;
    dados->users = users;
    dados->u = malloc(N * sizeof(struct user));
    int i;

    for(i=0;i < N; i++) {
        dados->u[i] = malloc(sizeof(struct user));
        dados->u[i]->count = -1;
        dados->u[i]->id = 0;
    }
    

    g_hash_table_foreach(coms->catalogo_commits,calculaCommitsAmigoOwner,dados);
    g_hash_table_foreach(dados->counts,getTopUsersAmigoOwner,dados);
    if(tipo == 0){
        // ficheiro
        for(i=0;i<N;i++) {
        int key=dados->u[i]->id;
        GH_USER u = lookupUser(users,key);
        char *login = strdup(getLogin(u));
        fprintf(f,"%d; %s\n",key,login);
        }
        return NULL;
    } else {
        GArray* arrayStrings = g_array_new(FALSE, FALSE, sizeof(char*));
        for(i=0;i<N;i++) {
        int key=dados->u[i]->id;
        GH_USER u = lookupUser(users,key);
        char *login = strdup(getLogin(u));
        char str[50];
        sprintf(str,"| %d | %s |",key,login);
        char* aux = strdup(str);
        g_array_append_val(arrayStrings,aux);
        }
        return arrayStrings;
    }
}

// QUERY10

struct message {
    int user_id;
    int size;
};

struct dadosQuery10 {
    int NU;
    int tipo;
    FILE *file;
    C_users users;
    GArray* arrayStrings;
};


void filtraMaioresMsgRepo(gpointer key, gpointer value, gpointer user_data) {
    DADOSQUERY10 dados = (DADOSQUERY10) user_data;
    char* aux;
    GArray *arrComs = (GArray *) value;
    GH_COMMIT com;
    if(arrComs->len < dados->NU) { 
        MESSAGE m[arrComs->len];
        int i;
        for(i=0;i<arrComs->len;i++) {
            m[i] = malloc(sizeof(struct user));
            m[i]->size = -1;
            m[i]->user_id = 0;
        }

        for(i=0;i<arrComs->len;i++) {
        com = g_array_index(arrComs, GH_COMMIT,i);
        int len = getCommitMessage(com);
        int a;
        for(a=0; a < (arrComs->len) && m[a]->size > len && m[a]->user_id!=get_Committer_Id(com) ;a++) {}
        if(a != arrComs->len) {
            if(m[a]->user_id!=get_Committer_Id(com)) {
            for(int j = (arrComs->len) - 1; j > a; j--) {
                m[j]->size = m[j-1]->size;
                m[j]->user_id = m[j-1]->user_id;
            }}
            m[a]->size=len;
            m[a]->user_id = get_Committer_Id(com);
        } 
        }
        int *repo_id = (int*) key;
        if(dados->tipo == 0){
            // ficheiro
            for(i = 0; i < arrComs->len && m[i]->user_id!=0; i++){
            GH_USER u = lookupUser(dados->users,(m[i]->user_id));
            fprintf(dados->file,"%d;%s;%d;%d\n",m[i]->user_id,getLogin(u),m[i]->size,(*repo_id));
            }
        } else {
            char strRepo[50];
            sprintf(strRepo,"------- Repositorio %d -------",*repo_id);
            aux = strdup(strRepo);
            g_array_append_val(dados->arrayStrings, aux);
            for(i = 0; i < arrComs->len && m[i]->user_id!=0; i++){
                GH_USER u = lookupUser(dados->users,(m[i]->user_id));
                char str[100];
                sprintf(str,"| %d | %s | %d | %d",m[i]->user_id,getLogin(u),m[i]->size,(*repo_id));
                aux = strdup(str);
                g_array_append_val(dados->arrayStrings, aux);
            }
        }
    }

    else {
        MESSAGE m[dados->NU];
        int i;
        for(i=0;i<dados->NU;i++) {
            m[i] = malloc(sizeof(struct user));
            m[i]->size = -1;
            m[i]->user_id = 0;
        }

        for(i=0;i<arrComs->len;i++) {
        com = g_array_index(arrComs, GH_COMMIT,i);
        int len = getCommitMessage(com);
        int a;
        for(a=0; a < (dados->NU) && m[a]->size > len && m[a]->user_id!=get_Committer_Id(com) ;a++) {}
        if(a != dados->NU) {
            if(m[a]->user_id!=get_Committer_Id(com)) {
            for(int j = (dados->NU) - 1; j > a; j--) {
                m[j]->size = m[j-1]->size;
                m[j]->user_id = m[j-1]->user_id;
            }}
            m[a]->size=len;
            m[a]->user_id = get_Committer_Id(com);
        }
        }
        int *repo_id = (int*) key;
        
        if(dados->tipo == 0){
            for(i = 0; i < dados->NU && m[i]->user_id!=0 ; i++){
                GH_USER u = lookupUser(dados->users,(m[i]->user_id));
                fprintf(dados->file,"%d;%s;%d;%d\n",m[i]->user_id,getLogin(u),m[i]->size,(*repo_id));
        }
        } else {
            char strRepo[50];
            sprintf(strRepo,"------- Repositorio %d -------",*repo_id);
            aux = strdup(strRepo);
            g_array_append_val(dados->arrayStrings, aux);
            for(i = 0; i < dados->NU && m[i]->user_id!=0 ; i++){
                GH_USER u = lookupUser(dados->users,(m[i]->user_id));
                char str[100];
                sprintf(str,"| %d | %s | %d | %d",m[i]->user_id,getLogin(u),m[i]->size,(*repo_id));
                aux = strdup(str);
                g_array_append_val(dados->arrayStrings, aux);
            }
        }
    }
}

GArray* determinaUtilizadoresMaiorMsg(C_users users,C_commits commits,int N,FILE *f,int tipo) {
    DADOSQUERY10 dados = malloc(sizeof(struct dadosQuery10));
    dados->NU = N;
    dados->file =f;
    dados->users = users;
    dados->arrayStrings = g_array_new(FALSE, FALSE, sizeof(char*));
 
    dados->tipo = tipo;
    g_hash_table_foreach(commits->catalogo_commits,filtraMaioresMsgRepo,dados);
    if(tipo == 0) return NULL;
    else return dados->arrayStrings;
}


// VALIDATE & START

struct validaCommits {
    C_users users;
    C_repos repos;
};

gboolean filtraCommit(gpointer key, gpointer value, gpointer userdata){
    GArray *commits= value;
    VALIDACOMS dados = (VALIDACOMS) userdata;
    GH_COMMIT com;
    int committer_id, author_id, repo_id;
    
    for(int i=commits->len-1;i>=0;i--) {
        com=g_array_index(commits,GH_COMMIT,i);
        committer_id=get_Committer_Id(com);
        author_id=get_author_id(com);
        repo_id=get_repo_id(com);
        if(tableContainsUser(dados->users,committer_id)==FALSE || tableContainsUser(dados->users,author_id)==FALSE ||  tableContainsRepo(dados->repos,repo_id)==FALSE){
            g_array_remove_index_fast(commits,i);
            if(commits->len == 0){
                return TRUE;
            }
        }
    }
    return FALSE;
}

void filtraCommits(C_commits coms,C_users users, C_repos repos){
    VALIDACOMS dados=malloc(sizeof(struct validaCommits));
    dados->users = users;
    dados->repos = repos;

    g_hash_table_foreach_steal(coms->catalogo_commits,filtraCommit,dados);
}