/* gitc: git-curses
 * A TUI frontend for the Git VCS
 * License:TBD
 * Author:Aditya Visvanathan
 * Version : 1.0.0
 */


#include "gitc.h"
#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>
int main(int argc,char **argv)
{
    WINDOW *main_win;
    setlocale(LC_ALL,"C");
    if ( ! check_if_repo() )
    {
        printf("gitc : Fatal! Not a Git Repository!\n");
        return E_EXIT;
    }

    main_win = initscr();
    curs_set(0);
    cbreak();
    keypad(main_win,TRUE);
    noecho();

    if( print_welc_scr(main_win) == E_EXIT )
    {
        curs_set(1);
        nocbreak();
        endwin();
        return E_EXIT;
    }

    repo_commit_menu(main_win);    
    curs_set(1); 
    nocbreak();
    endwin();

    return E_SUCCESS;
}
