#ifndef COMMITS_H
#define COMMITS_H

#include <glib-2.0/glib.h>

typedef struct gh_commit* GH_COMMIT;

int get_repo_id(GH_COMMIT com);

int get_author_id(GH_COMMIT com);

int get_committer_id(GH_COMMIT com);

int commitIsValid (GH_COMMIT gc);

int add_commit2file(GH_COMMIT gc, FILE *f);

void print_commit (GH_COMMIT gc);

GH_COMMIT build_commit(char* line);

GArray * load_commits (char *in, char *out);

#endif