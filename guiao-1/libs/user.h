#ifndef USER_H
#define USER_H

#include <glib-2.0/glib.h>

typedef struct gh_user* GH_USER;

typedef struct catalogo_users* C_users;

int countOccurrences (char *a,char ch);

int ValidDateFormat(int ano, int mes, int dia);

int ValidDate (struct tm date);

char* list_toString(int a[],int b);

void print_user (GH_USER u);

int add_user2file(GH_USER u, FILE *f);

int userIsValid (GH_USER u);

GH_USER build_user(char* line);

GHashTable *load_users (char *in, char *out);

#endif