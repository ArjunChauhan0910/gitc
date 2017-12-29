/* Headers for gitc
 * Author:Aditya Visvanathan
 * License:TBD
 */
#define VER "Version:1.0.0"
#define CLEAN_EXIT_WELC_SCR 1
#define CLEAN_EXIT_OPEN_ERR 2
#define CLEAN_EXIT_REPO_DET_SCR 3
#define ERR_EXIT_WELC_SCR -1
#define ERR_EXIT_OPEN_ERR -2
#define ERR_EXIT_REPO_DET_SCR -3
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <git2.h>

/* print welcome screen message window */
int print_welc_scr(WINDOW*);

/* check for valid git repo */
bool check_if_repo();

/* print error if git repo not found */
int print_git_repo_error(WINDOW*);

/* grab repo details */
int repo_commit_details_init(WINDOW*);

int is_init_key(int*);

