/* gitc: git-curses
 * A TUI frontend for the Git VCS
 * License:TBD
 * Author:Aditya Visvanathan
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
    setlocale(LC_ALL,"C");
    if ( ! check_if_repo() )
    {
        printf("Fatal! Not a Git Repository!\n");
        return EXIT_MAIN_INCOMPLETE;
    }

    initscr();
    curs_set(0);
    cbreak();
    keypad(stdscr,TRUE);
    noecho();

    int key = 0;
    if( print_welc_scr(stdscr) == 0 )
    {
        curs_set(1);
        endwin();
        return 1;
    }

    repo_commit_menu(stdscr);    
    while( (key = getch() ) != EXIT_KEY_PRESS && check_if_repo() )
    {
        repo_commit_menu(stdscr);        
    }  
    curs_set(1); 
    endwin();

    return CLEAN_EXIT_MAIN;
}
