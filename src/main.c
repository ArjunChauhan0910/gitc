/* gitc: git-curses
 * A TUI frontend for the Git VCS
 * License:TBD
 * Author:Aditya Visvanathan
 *Licence: MIT
 * Version : 1.0.0
 */
#define EXIT_KEY_PRESS 113
#define CLEAN_EXIT_MAIN 0


#include "gitc.h"
#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>
#define EXIT_MAIN_INCOMPLETE 1
int main(int argc,char **argv)
{
    WINDOW *main_win;
    setlocale(LC_ALL,"C");
    if ( ! check_if_repo() )
    {
        printf("gitc : Fatal! Not a Git Repository!\n");
        return EXIT_MAIN_INCOMPLETE;
    }

    main_win = initscr();
    curs_set(0);
    cbreak();
    keypad(main_win,TRUE);
    noecho();

    int key = 0;
    if( print_welc_scr(main_win) == 0 )
    {
        curs_set(1);
        nocbreak();
        endwin();
        return 1;
    }

    repo_commit_menu(main_win);    
    curs_set(1); 
    nocbreak();
    endwin();

    return CLEAN_EXIT_MAIN;
}
