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
    char cwd[256];
    initscr();
    noecho();
    int key;
    if( ! check_if_repo())
        print_git_repo_error(stdscr);
    else
      //  print_welc_scr(stdscr);
    while( (key = getch() ) != 113 )
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
                repo_commit_details_init(stdscr);
                

            }

        }   
        
    }   
    endwin();

    return 0;
}
