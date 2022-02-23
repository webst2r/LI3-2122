#ifndef REPOS_H
#define REPOS_H

#include <glib-2.0/glib.h>

typedef struct gh_repos* GH_REPOS;

typedef struct cat_repos* C_repos;

int get_OwnerId(GH_REPOS r);

GHashTable * get_catalogoRepos(C_repos crepos);

char * getReposLanguage(GH_REPOS r);

int get_Size_Repos (C_repos c);

char * get_updatedDate (GH_REPOS r);

char * get_Creat_at (GH_REPOS r);

char* get_RepoDescription (GH_REPOS r);

int get_RepoId (GH_REPOS r);

C_repos init_repos(C_repos rep);

void free_repos(C_repos rep);

GH_REPOS build_repos(char* line);

void rmRepos(gpointer res);

void rmKeyRepos(gpointer key);

void load_catalogo_repos(C_repos r, char * reps);

GHashTable * load_repos (char *in);

#endif