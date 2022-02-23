#ifndef SGG_H
#define SGG_H

#include <glib-2.0/glib.h>
#include "user.h"
#include "commits.h"
#include "repos.h"

typedef struct sgg *SGG;
typedef struct dadosQuery5 *DADOSQUERY5;
typedef struct dadosQuery6 *DADOSQUERY6;
typedef struct dadosQuery7 *DADOSQUERY7;
typedef struct dadosQuery8 *DADOSQUERY8;
typedef struct dadosQuery9 *DADOSQUERY9;
typedef struct dadosQuery10 *DADOSQUERY10;
typedef struct atividade *ATIVIDADE;
typedef struct value *VALUE;
typedef struct linguagem *LINGUAGEM;
typedef struct committer *COMMITTER;
typedef struct user *USER;
typedef struct message *MESSAGE;

SGG init_sgg();

void free_sgg(SGG sgg);

SGG load_sgg (char *fusers, char *fcoms, char *frepos);

typedef struct gh_query *GH_QUERY;

GH_QUERY build_query(char *line);

void lerFicheiroQueries(char *fileQueries,SGG sgg);

#endif