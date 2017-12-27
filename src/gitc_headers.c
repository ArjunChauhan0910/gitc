/* Function headers for gitc
 * Author:Aditya Visvanathan
 * License:TBD
 */
#define VER "Version:1.0.0"
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <git2.h>

/* function to print welcome screen message */
int print_welc_scr(WINDOW *win)
{
    /* check if window initialized correctly */
    if ( ! win )
        return -1;

    /* get window size */
    int row,col;
    getmaxyx(win,row,col);

    /* create fancy UI elements */
    box(win,0,0);
    wrefresh(win);
    static const char* title_msg_top = "gitc : Git-Curses";
    static const char* des_msg_centre = "A TUI frontend for the Git Version Control System";
    static const char* fol_msg = "Press any key to continue..";
    static const char* exit_msg = "Press Q to quit";
    mvwprintw(win,1,(col-strlen(title_msg_top))/2,"%s",title_msg_top);
    mvwprintw(win,row/2,(col-strlen(des_msg_centre))/2,"%s",des_msg_centre);
    mvwprintw(win,(row/2)+1,(col-strlen(VER))/2,"%s",VER);
    mvwprintw(win,(row/2)+2,(col-strlen(fol_msg))/2,"%s",fol_msg);
    mvwprintw(win,(row-2),(col-strlen(exit_msg))/2,"%s",exit_msg);
    wrefresh(win);
    return 0;
}

/* Function to check if 'path' is a git repo */
bool check_if_repo()
{
    /* initialize libgit2 */
    git_libgit2_init();
    git_repository *cur_repo = NULL;
    int error;
    /* try to open git repo in 'path' if possible, fails if not found */
    error = git_repository_open_ext(&cur_repo,".",0,NULL);
    /* deinitialize libgit2 cause job done */
    git_repository_free(cur_repo);
    git_libgit2_shutdown();

    /* return true only if repo opened successfully */
    return error == 0;
}

/* func to print error message if no git repo found */
int print_git_repo_error(WINDOW *win)
{
    /* Check for window errors */
    if ( ! win )
        return -1;

    /* messges to display */
    static const char* git_dir_err_msg = "Fatal! Not a git repository!";
    static const char* bottom_msg = "Press Q to quit";
    
   /* grab window borders */
    int row,col;
    getmaxyx(win,row,col);

    /* print both messages */
    clear();
    mvwprintw(win,row/2,(col-strlen(git_dir_err_msg))/2,"%s",git_dir_err_msg);
    mvwprintw(win,row-2,(col-strlen(bottom_msg))/2,"%s",bottom_msg);
    
    /* refresh window to take effect */
    wrefresh(win);

    /* successful exit */
    return 0;
}

int repo_commit_details_init(WINDOW *win)
{
    if ( ! win )
        return -1;
    static const char *commit_title_text = "Commit details";
    git_libgit2_init();
    git_repository *root_repo = NULL;
    git_revwalk *walker = NULL;
    int open_error = git_repository_open_ext(&root_repo,".",0,NULL);
    git_revwalk_new(&walker,root_repo);
    git_revwalk_sorting(walker,GIT_SORT_NONE);
    git_revwalk_push_head(walker);
    git_oid commit_id;
    int lc = 0;
    while( ! git_revwalk_next(&commit_id,walker) )
    {
        git_commit *commit_obj = NULL;
        git_commit_lookup(&commit_obj,root_repo,&commit_id);
        wclear(win);
        mvwprintw(win,lc,0,"%s",git_oid_tostr_s(&commit_id));
        wrefresh(win);
        lc++;
        git_commit_free(commit_obj);
    }
    git_revwalk_free(walker);
    git_repository_free(root_repo);
    git_libgit2_shutdown();
    return 0;
                

}

