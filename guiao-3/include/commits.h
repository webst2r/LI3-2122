#ifndef COMMITS_H
#define COMMITS_H

#include <glib-2.0/glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



typedef struct validaCommits* VALIDACOMS;
typedef struct gh_commit* GH_COMMIT;
typedef struct c_commits* C_commits;
#include "../include/repos.h"
#include "../include/user.h"

int get_NColaboradores();

int get_Repos_with_Bots();

int getCommitMessage(GH_COMMIT c);

char * get_commit_data(GH_COMMIT c);

int get_nCommits ();

int get_author_id(GH_COMMIT com);

int get_repo_id(GH_COMMIT com);

gboolean tableContainsCommit(C_commits coms,int id);

GArray* lookupCommit(C_commits coms, int *id);

int get_Committer_Id (GH_COMMIT c);

C_commits init_commits();

GH_COMMIT build_commit(char* line);

void load_catalogo_commits(C_commits c, char * coms,C_users us);

GHashTable * load_commits (char *in,C_users us);

void getTopMaisAtivos (gpointer key, gpointer value, gpointer userData);

void calculaAtividade (gpointer key, gpointer value, gpointer userData);

GArray* calculaUtilizadoresMaisAtivos(C_commits coms,C_users users,int N,char *dataInicio,char *dataFim,FILE *f, int tipo);
typedef struct dadosQuery5 *DADOSQUERY5;
typedef struct atividade *ATIVIDADE;
typedef struct value *VALUE;

GArray* calculaUtilizadoresMaisCommitsLinguagem(C_commits coms,C_users users,C_repos repos,int N,char *language, FILE *f, int tipo);
typedef struct dadosQuery6 *DADOSQUERY6;
typedef struct committer *COMMITTER;

GArray* determinaReposInativos(C_commits coms, C_repos repos, char *dataInicio,FILE *f, int tipo);
typedef struct dadosQuery7 *DADOSQUERY7;

GArray* determinaTopUsersAmigoOwner(C_commits coms,C_repos repos,C_users users,int N,FILE *f, int tipo);
typedef struct dadosQuery9 *DADOSQUERY9;
typedef struct user *USER;

GArray* determinaUtilizadoresMaiorMsg(C_users users,C_commits commits,int N,FILE *f,int tipo);
typedef struct dadosQuery10 *DADOSQUERY10;
typedef struct message *MESSAGE;

void filtraCommits(C_commits coms,C_users users, C_repos repos);

#endif
