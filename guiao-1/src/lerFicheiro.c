#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include "../libs/lerFicheiro.h"

void exercicio1(){
    load_users("entrada/users.csv","saida/users-ok.csv");
    load_commits("entrada/commits.csv","saida/commits-ok.csv");
    load_repos("entrada/repos.csv","saida/repos-ok.csv");
}

static gboolean verifica_user_repos (gpointer key, gpointer value, gpointer user_data) {
    GH_REPOS r=(GH_REPOS) value;
    int i=get_owner_id(r);
    if(g_hash_table_contains(user_data,&i)==FALSE) {
        return TRUE;
    }
    else {return FALSE;}
}

static void writefinal (gpointer key, gpointer value, gpointer file) {
    add_repos2file(value,file);
}

void exercicio2() {
    GHashTable * users = load_users("saida/users-ok.csv","saida/users-final.csv");
    GArray* commits = load_commits("saida/commits-ok.csv","saida/commits-final.csv");
    GHashTable * repos = load_repos("saida/repos-ok.csv","saida/repos-final.csv");
    GH_COMMIT com;
    GH_REPOS rep;
    int committer_id,author_id,repo_id,i;
    //commits
    printf("Gerando ficheiro commits-final.csv\n");
    for(i=commits->len-1;i>=0;i--) {
        com=g_array_index(commits,GH_COMMIT,i);
        committer_id=get_committer_id(com);
        author_id=get_author_id(com);
        repo_id=get_repo_id(com);
        if(g_hash_table_contains(users,&committer_id)==FALSE || g_hash_table_contains(users,&author_id)==FALSE ||  g_hash_table_contains(repos,&repo_id)==FALSE){
            g_array_remove_index_fast(commits,i);
        }
    }
    FILE *outCom=fopen("saida/commits-final.csv","w");
    FILE *outRep=fopen("saida/repos-final.csv","w");
    char *cab_com="repo_id;author_id;committer_id;commit_at;message\n";
    char *cab_rep ="id;owner_id;full_name;license;has_wiki;description;language;default_branch;created_at;updated_at;forks_count;open_issues;stargazers_count;size\n";
    fputs(cab_com,outCom);
    fputs(cab_rep,outRep);

    for(i=0;i<commits->len;i++) {
        com=g_array_index(commits,GH_COMMIT,i);
        add_commit2file(com,outCom);
    }
    printf("Ficheiro commits-final.csv finalizado\n\n");

    //repos
    printf("Gerando ficheiro repos-final.csv\n");
    g_hash_table_foreach_steal (repos, verifica_user_repos, users);
    
    GHashTable * aux  = g_hash_table_new_full(g_int_hash,g_int_equal,rmKeyRepos,rmRepos);
    for(int i=0;i<commits->len;i++) {
        com=g_array_index(commits,GH_COMMIT,i); 
        repo_id=get_repo_id(com);
        rep=g_hash_table_lookup(repos,&repo_id);
        if(rep!=NULL && g_hash_table_contains(aux,&repo_id)==FALSE) {
                g_hash_table_insert(aux,&repo_id,rep);
        }
        }
    g_hash_table_foreach (aux,writefinal, outRep);
    printf("Ficheiro repos-final.csv finalizado\n");
    fclose(outCom);
    fclose(outRep);
}

