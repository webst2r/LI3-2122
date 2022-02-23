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

// Dado o caminho para os 3 ficherios (users, commits, repos), ler o conteúdo e carregar as estruturas de dados
SGG load_sgg (char *fusers, char *fcoms, char *frepos){
    SGG sgg = init_sgg();
    load_catalogo_users(sgg->users,fusers);
    load_catalogo_repos(sgg->repos,frepos);
    load_catalogo_commits(sgg->commits,fcoms,get_Table_User(sgg->users));
    return sgg;
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

//Query2
void query2(SGG sgg,FILE *f) {
    float f1= get_NColaboradores();
    float f2= get_Size_Repos(sgg->repos);
    fprintf(f,"%.2f\n\n",f1/f2);
}

//Query3
void query3(FILE *f) {fprintf(f,"%d\n\n",get_Repos_with_Bots());}

//Query4
void query4(SGG sgg,FILE *f) {
    float f1 = get_nCommits();
    float f2 = get_Size_User(sgg->users);
    fprintf(f,"%.2f\n\n",f1/f2);
}

int posterior (char *date1,char *date2) {
    int ano1=atoi(strsep(&date1,"-"));
    int ano2=atoi(strsep(&date2,"-"));
    if(ano1<ano2) {return 0;}
    int mes1= atoi(strsep(&date1,"-"));
    int mes2= atoi(strsep(&date2,"-"));
    if(mes1<mes2) {return 0;}
    int dia1= atoi(strsep(&date1,""));
    int dia2= atoi(strsep(&date2,""));
    if(dia1<=dia2) {return 0;}
    return 1;  
}

//Query5
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


void auxQuery5 (gpointer key, gpointer value, gpointer userData) {
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
        char inicio[12];
        char fim[12];
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

void topN (gpointer key, gpointer value, gpointer userData) {
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

void query5(SGG sgg,int N,char *dataInicio,char *dataFim,FILE *f) {//take(dados->counts,get_catalogoUsers(sgg->users),N);
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
    g_hash_table_foreach(get_catalogoCommits(sgg->commits),auxQuery5,dados);
    g_hash_table_foreach(dados->counts,topN,dados); 
    for(i=0;i<N;i++) {
        int key=dados->a[i]->id;
        GH_USER u = g_hash_table_lookup(get_catalogoUsers(sgg->users),&key);
        char *login = strdup(getLogin(u));
        fprintf(f,"%d; %s; %d\n",key,login,dados->a[i]->ocorrencias);
    }
}


//Query6
struct committer {
    int idCommitter;
    int count;
};

struct dadosQuery6 {
    int NU;
    char *linguagem;
    GHashTable *counts;
    GHashTable *repos;
    COMMITTER *c;
};

char* toLower(char* s) {
  for(char *p=s; *p; p++) *p=tolower(*p);
  return s;
}

void auxQuery6 (gpointer key, gpointer value, gpointer userData) {
    DADOSQUERY6 dados = (DADOSQUERY6) userData;
    GArray *arrComs = (GArray *) value;
    GH_COMMIT com;
    GH_REPOS repo;
    int committerId;
    VALUE valor;

    repo = g_hash_table_lookup(dados->repos,key);
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


void take_top (gpointer key, gpointer value, gpointer userData) {
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


void query6(SGG sgg,int N,char *language, FILE *f) {
    DADOSQUERY6 dados=malloc(sizeof(struct dadosQuery6));
    dados->NU = N;
    dados->linguagem = language;
    dados->counts = g_hash_table_new(g_int_hash,g_int_equal);
    dados->repos = get_catalogoRepos(sgg->repos);
    dados->c = malloc(N * sizeof(struct committer)); 
    int i;
    for(i=0;i<N;i++) {
        dados->c[i]= malloc(sizeof(struct committer));
        dados->c[i]->count = -1;
        dados->c[i]->idCommitter = 0;
    }

    g_hash_table_foreach(get_catalogoCommits(sgg->commits),auxQuery6,dados);
    g_hash_table_foreach(dados->counts,take_top, dados);    
    for(i=0;i<N;i++) {
        int key = dados->c[i]->idCommitter;
        GH_USER u = g_hash_table_lookup(get_catalogoUsers(sgg->users), &key);
        char *login = strdup(getLogin(u));
        fprintf(f,"%d;%s;%d\n",dados->c[i]->idCommitter,login,dados->c[i]->count);
    }  
}
//Query7
struct dadosQuery7 { 
    char *data_inicio;
    FILE *out;
    GHashTable *reposit;
};

void auxQuery7 (gpointer key, gpointer value, gpointer userData) {
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
        if(r==0) {
        GH_REPOS  repositorio = g_hash_table_lookup(dados->reposit,key); 
        char* des = get_RepoDescription (repositorio);
        int *id=key;
        fprintf(dados->out,"%d;%s\n",(*id),des);
    }
}

void query7(SGG sgg,char *dataInicio,FILE *f) {
    DADOSQUERY7 dados = malloc(sizeof(struct dadosQuery7));
    dados ->data_inicio=dataInicio;
    dados->out =f;
    dados->reposit = get_catalogoRepos(sgg->repos);
    g_hash_table_foreach(get_catalogoCommits(sgg->commits),auxQuery7,dados);
}

//Query8
struct dadosQuery8 { 
    char *data_inicio;
    int NU;
    GHashTable *counts;
    LINGUAGEM *l;
};

struct linguagem {
    char *linguagem;
    int ocorrencias;
};

void auxQuery8 (gpointer key, gpointer value, gpointer userData) {
    DADOSQUERY8 dados=(DADOSQUERY8) userData;
    GH_REPOS repo = (GH_REPOS) value;
    char date[12];
    char *date2 = strdup(dados->data_inicio);
    char *chave;
    VALUE valor;
    strcpy(date,get_updatedDate(repo));
    
    if(posterior(date,date2)==0) {
        chave = getReposLanguage(repo);
        valor= g_hash_table_lookup(dados->counts,chave);
        if( valor == NULL){
            VALUE new = malloc(sizeof(struct value));
            new->num=1;
            g_hash_table_insert(dados->counts,chave,new);
        } else {
            valor->num=valor->num+1;
            g_hash_table_insert(dados->counts,chave,valor);
        }
    } 
}

void busca_top (gpointer key, gpointer value, gpointer userData) {
    DADOSQUERY8 dados=(DADOSQUERY8) userData;
    VALUE v = (VALUE) value;
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

void query8(SGG sgg,int N,char *dataInicio,FILE *f) {
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
    g_hash_table_foreach(get_catalogoRepos(sgg->repos),auxQuery8,dados);
    g_hash_table_foreach(dados->counts,busca_top,dados); 
    for(i=0;i<N;i++) {
        fprintf(f,"%s; %d\n",dados->l[i]->linguagem,dados->l[i]->ocorrencias);
    }
}

//Query9
struct user {
    int id;
    int count;
};

struct dadosQuery9 {
    int NU;
    GHashTable *counts;
    GHashTable *repos;
    GHashTable *users;
    USER *u;
};

int isFriend (GH_USER owner, GH_USER committer){
    // owner está contido na lista de followers e following do committer
    int ownerId = getUserId(owner);
    int *following_list = getFollowingList(committer);
    int following = getFollowing(committer);
    int *followers_list = getFollowersList(committer);
    int followers = getFollowers(committer);

    int pertenceFollowers = 0;
    int pertenceFollowing = 0;
    // percorrer os following
    for(int i = 0; i < following; i++){
        if(following_list[i] == ownerId){
            pertenceFollowing = 1;
        }
    }
    // percorrer os followers
    for(int i = 0; i < followers; i++){
        if(followers_list[i] == ownerId){
            pertenceFollowers = 1;
        }
    }

    if(pertenceFollowing == 1 && pertenceFollowers == 1) return 1;
    else return 0;
}

void auxQuery9 (gpointer key, gpointer value, gpointer userData) {
    DADOSQUERY9 dados = (DADOSQUERY9) userData;
    GArray *arrComs = (GArray *) value;
    GH_COMMIT com;
    int committerID, ownerID;
    GH_REPOS repo;
    VALUE valor;

    for(int i = 0; i < arrComs->len; i++) {
        com = g_array_index(arrComs, GH_COMMIT, i);
        repo = g_hash_table_lookup(dados->repos, key);

        if(repo != NULL) {
            ownerID = get_OwnerId(repo);
            committerID = get_Committer_Id(com);
            GH_USER owner = g_hash_table_lookup(dados->users, &ownerID);
            GH_USER committer = g_hash_table_lookup(dados->users, &committerID);
            
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



void take_top_query9(gpointer key, gpointer value, gpointer userData) {
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

void query9(SGG sgg,int N,FILE *f) {
    DADOSQUERY9 dados = malloc(sizeof(struct dadosQuery9));
    dados->NU = N;
    dados->counts = g_hash_table_new(g_int_hash,g_int_equal);
    dados->repos = get_catalogoRepos(sgg->repos);
    dados->users = get_catalogoUsers(sgg->users);
    dados->u = malloc(N * sizeof(struct user));
    int i;

    for(i=0;i < N; i++) {
        dados->u[i] = malloc(sizeof(struct user));
        dados->u[i]->count = -1;
        dados->u[i]->id = 0;
    }
    

    g_hash_table_foreach(get_catalogoCommits(sgg->commits),auxQuery9,dados);
    g_hash_table_foreach(dados->counts,take_top_query9,dados);
    for(i=0;i<N;i++) {
        int key=dados->u[i]->id;
        GH_USER u = g_hash_table_lookup(get_catalogoUsers(sgg->users),&key);
        char *login = strdup(getLogin(u));
        fprintf(f,"%d; %s\n",key,login);
    }

}

//Query10
struct message {
    int user_id;
    int size;
};

struct dadosQuery10 {
    int NU;
    GHashTable *users;
    FILE *file;
};


void auxQuery10(gpointer key, gpointer value, gpointer user_data) {
    DADOSQUERY10 dados = (DADOSQUERY10) user_data;
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
        char *msg = getCommitMessage(com);
        int len = strlen(msg);
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
        for(i = 0; i < arrComs->len && m[i]->user_id!=0; i++){
        GH_USER u = g_hash_table_lookup(dados->users,&(m[i]->user_id));
        fprintf(dados->file,"%d;%s;%d;%d\n",m[i]->user_id,getLogin(u),m[i]->size,(*repo_id));
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
        char *msg = getCommitMessage(com);
        int len = strlen(msg);
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
        for(i = 0; i < dados->NU && m[i]->user_id!=0 ; i++){
        GH_USER u = g_hash_table_lookup(dados->users,&(m[i]->user_id));
        fprintf(dados->file,"%d;%s;%d;%d\n",m[i]->user_id,getLogin(u),m[i]->size,(*repo_id));
        }

    }
}


void query10(SGG sgg,int N,FILE *f) {
    DADOSQUERY10 dados = malloc(sizeof(struct dadosQuery10));
    dados->NU = N;
    dados->file=f;
    dados->users=get_catalogoUsers(sgg->users);
    g_hash_table_foreach(get_catalogoCommits(sgg->commits),auxQuery10, dados);
}

//parsing
GH_QUERY build_query(char *line){
    GH_QUERY query = malloc(sizeof(struct gh_query));
    char *buffer2 = line;

    /* Numero da Query */
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
            query5(sgg,q->N,q->data_inicio,q->data_fim,f);
            break;
        case 6:
            query6(sgg,q->N,q->language,f);
            break;
        case 7:
            query7(sgg,q->data_inicio,f);
            break;
        case 8:
            query8(sgg,q->N,q->data_inicio,f);
            break;
        case 9:
            query9(sgg,q->N,f);
            break;
        case 10:
            query10(sgg,q->N,f);
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
    char *name="saida/comand";
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
