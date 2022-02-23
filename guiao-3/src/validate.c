#include "../include/commits.h"
#include "../include/user.h"
#include "../include/repos.h"
#include "../include/validate.h"



SGG validateAndStart () {
    // Users
    C_users cat_users = init_users();
    load_catalogo_users(cat_users,"entrada/users-g3.csv");
    
    // Repos
    C_repos cat_repos = init_repos();
    load_catalogo_repos(cat_repos,"entrada/repos-g3.csv");

    // Commits
    C_commits cat_commits = init_commits();
    load_catalogo_commits(cat_commits,"entrada/commits-g3.csv",cat_users);

    filtraCommits(cat_commits,cat_users,cat_repos);
    filtraRepos(cat_repos,cat_commits,cat_users);
    
    SGG sgg = init_sgg();
    set_SGG(sgg,cat_users,cat_commits,cat_repos);   
    return sgg;
}