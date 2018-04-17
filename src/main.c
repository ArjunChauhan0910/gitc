/* gitc: git-curses
 * A TUI frontend for the Git VCS
 * License:TBD
 * Author:Aditya Visvanathan
 * Version : Testing, Codename : maloha
 */

#ifndef _GITC_H_
#include "gitc.h"
#endif
#include <locale.h>
int main(int argc,char **argv)
{
    
    /* set locale to C to speed up performance */
    setlocale(LC_ALL,"C");

    if ( argc > 1 )
    {
            if ( strcmp(argv[1],"--version") == 0 )
                print_gitc_ver();
    }

    else
    { 
        /* check if repo exists */
        if ( ! check_if_repo() )
        {
            fprintf(stdout,"gitc : Fatal! Not a Git Repository!\n");
            return E_EXIT;
        }
    
        /* initialize curses */
        WINDOW *main_win = initscr();
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
    }
    return E_SUCCESS;
}
