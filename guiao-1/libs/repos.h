#ifndef REPOS_H
#define REPOS_H

#include <glib-2.0/glib.h>

typedef struct gh_repos* GH_REPOS;

int reposIsValid (GH_REPOS rep);

int get_owner_id(GH_REPOS r);

int add_repos2file(GH_REPOS rep, FILE *f);

void print_repos (GH_REPOS rep);

GH_REPOS build_repos(char* line);

void rmRepos(gpointer res);

void rmKeyRepos(gpointer key);

GHashTable * load_repos (char *in, char *out);

#endif