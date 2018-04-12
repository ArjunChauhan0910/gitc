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
    /* set locale to C to speed up performance */
    setlocale(LC_ALL,"C");

    

    
    /* check if repo exists */
    if ( ! check_if_repo() )
    {
        fprintf(stdout,"gitc : Fatal! Not a Git Repository!\n");
        return E_EXIT;
    }
    
    /* initialize curses */
    main_win = initscr();
    curs_set(0);
    cbreak();
    keypad(main_win,TRUE);
    noecho();
    
    /* exit main app if user triggers exit at welcome screen */
    if( print_welc_scr(main_win) == E_EXIT )
    {
        curs_set(1);
        nocbreak();
        endwin();
        return E_EXIT;
    }
    /* continue with main app after main screen if exit is not triggered */
    repo_commit_menu(main_win);   

    /* clean up and exit curses on main app exit */  
    curs_set(1); 
    nocbreak();
    endwin();
   
    return E_SUCCESS;
}
