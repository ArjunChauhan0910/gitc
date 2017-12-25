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
    getcwd(cwd,sizeof(cwd));
    if( ! check_if_repo(cwd))
        print_git_repo_error(stdscr);
    else
        print_welc_scr(stdscr);
    while( (key = getch() ) != 113 )
    {
        if ( key == KEY_RESIZE )
        {
            clear();
            if ( ! check_if_repo(cwd) )
            {
                clear();
                print_git_repo_error(stdscr);
            }
            else
            {
                print_welc_scr(stdscr);
                wrefresh(stdscr);
            }

        }   
        
    }   
    endwin();

    return 0;
}
