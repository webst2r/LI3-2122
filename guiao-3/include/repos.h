#ifndef REPOS_H
#define REPOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>



typedef struct gh_repos* GH_REPOS;
typedef struct cat_repos* C_repos;
#include "../include/user.h"
#include "../include/commits.h"
typedef struct dadosQuery8 *DADOSQUERY8;
typedef struct linguagem *LINGUAGEM;
typedef struct validaRepos* VALIDAREPOS;



int get_OwnerId(GH_REPOS r);

GH_REPOS lookupRepo(C_repos repos, int *id);

gboolean tableContainsRepo(C_repos repos,int id);

char * getReposLanguage(GH_REPOS r);

int get_Size_Repos (C_repos c);

char * get_updatedDate (GH_REPOS r);

char * get_Creat_at (GH_REPOS r);

char* get_RepoDescription (GH_REPOS r);

int get_RepoId (GH_REPOS r);

C_repos init_repos();

void free_repos(C_repos rep);

GH_REPOS build_repos(char* line);

void rmRepos(gpointer res);

void rmKeyRepos(gpointer key);

void load_catalogo_repos(C_repos r, char * reps);

GHashTable * load_repos (char *in);

GArray* determinaLinguagensMaisUtilizadas(C_repos repos,int N,char *dataInicio,FILE *f, int tipo);
typedef struct valor *VALOR;

void filtraRepos(C_repos repos,C_commits coms,C_users users);


#endif