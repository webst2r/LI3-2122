#ifndef SGG_H
#define SGG_H

#include <glib-2.0/glib.h>
#include <stdio.h>
#include "user.h"
#include "commits.h"
#include "repos.h"

typedef struct sgg *SGG;

typedef struct gh_query *GH_QUERY;


SGG init_sgg();

void free_sgg(SGG sgg);

void set_SGG(SGG sgg, C_users users, C_commits commits,C_repos repos);

GH_QUERY build_query(char *line);

void lerFicheiroQueries(char *fileQueries,SGG sgg);

void printQuery1();
void printQuery2();
void printQuery3();
void printQuery4();

void query1(FILE *f);
void query2(SGG sgg,FILE *f);
void query3(FILE *f);
void query4(SGG sgg,FILE *f);
GArray* query5(SGG sgg,int N,char *dataInicio,char *dataFim,FILE *f, int tipo);
GArray* query6(SGG sgg,int N,char *language, FILE *f, int tipo);
GArray* query7(SGG sgg,char *dataInicio,FILE *f, int tipo);
GArray* query8(SGG sgg,int N,char *dataInicio,FILE *f, int tipo);
GArray* query9(SGG sgg,int N,FILE *f, int tipo);
GArray* query10(SGG sgg,int N,FILE *f,int tipo);

char* toLower(char* s);

#endif