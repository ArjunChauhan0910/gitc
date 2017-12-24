#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <git2.h>
int print_welc_scr(WINDOW *win)
{
    if ( ! win )
        return -1;
    int row,col;
    box(win,0,0);
    wrefresh(win);
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

bool check_if_repo(char *path)
{
    git_libgit2_init();
    git_repository *cur_repo;
    int repo_open_error = git_repository_open(&cur_repo,path);
    git_repository_free(cur_repo);
    git_libgit2_shutdown();
    return repo_open_error == 0;
}
int print_git_repo_error(WINDOW *win)
{
    static const char* git_dir_err_msg = "Fatal! Not a git repository!";
    static const char* bottom_msg = "Press Q to quit";
    int row,col;
    getmaxyx(win,row,col);
        clear();
        mvwprintw(win,row/2,(col-strlen(git_dir_err_msg))/2,"%s",git_dir_err_msg);
        mvwprintw(win,row-2,(col-strlen(bottom_msg))/2,"%s",bottom_msg);
        wrefresh(win);
    return 0;
}

