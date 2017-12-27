/* gitc: git-curses
 * A TUI frontend for the Git VCS
 * License:TBD
 * Author:Aditya Visvanathan
 * Version : 1.0.0
 */
#include "gitc_headers.h"
#include <ncurses.h>
#include <string.h>
#include <unistd.h>
int main(int argc,char **argv)
{
    initscr();
    noecho();
    int key = 1;
    if( ! check_if_repo())
        print_git_repo_error(stdscr);
    else
        print_welc_scr(stdscr);

    while( (key = getch()) != 113)
    {
        repo_commit_details_init(stdscr);
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
                print_welc_scr(stdscr);
            }

        }   
        
    }   
    endwin();

    return 0;
}
