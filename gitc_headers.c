#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int print_welc_scr(WINDOW *win)
{
    if ( ! win )
        return -1;
    box(win,0,0);
    wrefresh(win);
    int row,col;
    getmaxyx(win,row,col);
    static const char* title_msg_top = "gitc : Git-Curses";
    static const char* des_msg_centre = "A TUI frontend for the Git Version Control System";
    static const char* fol_msg = "Press any key to continue..";
    static const char* exit_msg = "Press Q to quit";
    mvwprintw(win,1,(col-strlen(title_msg_top))/2,"%s",title_msg_top);
    mvwprintw(win,row/2,(col-strlen(des_msg_centre))/2,"%s",des_msg_centre);
    mvwprintw(win,(row/2)+1,(col-strlen(fol_msg))/2,"%s",fol_msg);
    mvwprintw(win,(row-2),(col-strlen(exit_msg))/2,"%s",exit_msg);
    wrefresh(win);
    return 0;
}

