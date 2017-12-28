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
int main(int argc,char **argv)
{
    initscr();
    noecho();
    bool if_welc_scr_done = false;
    int key = 1;
    if( ! check_if_repo())
        print_git_repo_error(stdscr);
    else
        if_welc_scr_done = (print_welc_scr(stdscr) == 0 );

    while( (key = getch()) != 113)
    {
        if ( key == KEY_RESIZE )
        {
            clear();
            if ( ! check_if_repo() )
            {
                clear();
                print_git_repo_error(stdscr);
            }
            else
            {
                clear();
                if ( if_welc_scr_done )
                    repo_commit_details_init(stdscr);
                else
                    print_welc_scr(stdscr);
            }

        }
       if ( if_welc_scr_done )
          repo_commit_details_init(stdscr); 
        
    }   
    endwin();

    return 0;
}
