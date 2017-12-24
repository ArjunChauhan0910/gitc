/* Headers for gitc
 * License:TBD
 * Author:Aditya Visvanathan
 */

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <git2.h>
int print_welc_scr(WINDOW*);
bool check_if_repo(char*);
int print_git_repo_error(WINDOW*);
void init(char*,WINDOW*);
