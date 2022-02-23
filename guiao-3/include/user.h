#ifndef USER_H
#define USER_H

#include <glib-2.0/glib.h>

typedef struct gh_user* GH_USER;

typedef struct c_users* C_users;


int get_Size_User (C_users c);

int *getFollowersList(GH_USER u);

int getFollowers(GH_USER u);

int *getFollowingList(GH_USER u);

int getFollowing(GH_USER u);

int get_bot();

int get_organization();

int get_user();

char* getLogin(GH_USER u);

int getUserId(GH_USER u);

gboolean tableContainsUser(C_users users,int id);

void freeUser(GH_USER u);
void rmUser(gpointer usr);
void rmKeyUser(gpointer key);

C_users init_users();

void free_users(C_users us);

GH_USER lookupUser(C_users users, int id);

int isFriend (GH_USER owner, GH_USER committer);

int countOccurrences (char *a,char ch);

char* list_toString(int a[],int b);

GH_USER build_user(char* line);

void load_catalogo_users(C_users us, char * users);

GHashTable *load_users (char *in);


#endif