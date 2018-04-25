/* Headers for gitc
 * Author:Aditya Visvanathan
 * License:TBD
 */

/* begin include guards */
#ifndef _GITC_H_
#define _GITC_H_
#include <ncurses.h>
#include <git2.h>
#include <string.h>
#include <menu.h>
#include <stdlib.h>
#include <unistd.h>


/* enumeration for key press codes in curses mode */
typedef enum _KEYBIND 
{ 
    RES_KEY = KEY_RESIZE,
    UP_KEY = KEY_UP,
    DOWN_KEY = KEY_DOWN,
    ENTER_KEY = 10,
    VI_KEY_UP = 107,
    VI_KEY_DOWN = 106,
    EXIT_KEY = 113 } 
keybind;

/* enumeration for exit return codes througout gitc */
typedef enum _ERR_CODE { E_REPO_ERR = -2,
    E_WIN_ERR = -1,
    E_SUCCESS = 0,
    E_EXIT = 1 } err_code;


/* print welcome screen message window */
int print_welc_scr(WINDOW*);

/* check for valid git repo */
bool check_if_repo(void);

/* print error if git repo not found */
int print_git_repo_error(WINDOW*);

/* grab repo details */
int repo_commit_menu(WINDOW*);

int get_commit_count(void);

int wprint_text_mid(WINDOW*,const char*);

int print_gitc_ver(void);

/* a really optimized way to write to stdout that is shamelessly stolen from GNU yes */
int optimized_write_to_stdout(const char[]); 

char *const_to_str(const char*);
/* end include guards*/
#endif
