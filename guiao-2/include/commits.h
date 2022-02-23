#ifndef COMMITS_H
#define COMMITS_H

#include <glib-2.0/glib.h>

typedef struct gh_commit* GH_COMMIT;

int get_NColaboradores();

int get_Repos_with_Bots();

char *getCommitMessage(GH_COMMIT c);

char * get_commit_data(GH_COMMIT c);

typedef struct c_commits* C_commits;

int get_nCommits ();

GHashTable * get_catalogoCommits(C_commits cComs);

int get_Committer_Id (GH_COMMIT c);

C_commits init_commits(C_commits coms);

GH_COMMIT build_commit(char* line);

void load_catalogo_commits(C_commits c, char * coms,GHashTable *h);

GHashTable * load_commits (char *in,GHashTable *c);

#endif
