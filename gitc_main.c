#include "gitc_headers.h"
#include <ncurses.h>
#include <string.h>
int main()
{
    initscr();
    noecho();
    raw();
    int key;
    /* initial print_welc_scr */
    print_welc_scr(stdscr);
    while( (key = getch() ) != 113 )
    {
        if ( key == KEY_RESIZE )
        {
            clear();
            print_welc_scr(stdscr);
        }
        
    }
    endwin();
    return 0;
}
