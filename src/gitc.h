/* Headers for gitc
 * Author:Aditya Visvanathan
 * License:TBD
 */
#define VER "Version"
#ifndef GITC_H_
#define GITC_H_


#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <git2.h>
#include <menu.h>
/* print welcome screen message window */
int print_welc_scr(WINDOW*);

/* check for valid git repo */
bool check_if_repo();

/* print error if git repo not found */
int print_git_repo_error(WINDOW*);

/* grab repo details */
int repo_commit_menu(WINDOW*);

int get_commit_count();
#endif
