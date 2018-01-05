/* Function headers for gitc
 * Author:Aditya Visvanathan
 * License:TBD
 */
#define VER "Version:1.0.0"
#define CLEAN_EXIT_WELC_SCR 1
#define CLEAN_EXIT_OPEN_ERR 2
#define CLEAN_EXIT_REPO_DET_SCR 3
#define ERR_EXIT_WELC_SCR -1
#define ERR_EXIT_OPEN_ERR -2
#define ERR_EXIT_REPO_DET_SCR -3
#define EXIT_KEYPRESS_CODE 113

#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <git2.h>
#include <menu.h>
#include "gitc.h"

int print_welc_scr(WINDOW* win)
{
    if ( ! win )
        return ERR_EXIT_WELC_SCR;
    int row,col;
    getmaxyx(win,row,col);

    ;
    wrefresh(win);
    static const char* title_msg_top = "gitc : Git-Curses";
    static const char* des_msg_centre = "A TUI frontend for the Git Version Control System";
    static const char* fol_msg = "Press any key to continue..";
    static const char* exit_msg = "Press Q to quit";
    mvwprintw(win,1,(col-strlen(title_msg_top))/2,"%s",title_msg_top);
    mvwprintw(win,row/2,(col-strlen(des_msg_centre))/2,"%s",des_msg_centre);
    mvwprintw(win,(row/2)+2,(col-strlen(fol_msg))/2,"%s",fol_msg);
    mvwprintw(win,(row-2),(col-strlen(exit_msg))/2,"%s",exit_msg);
    wrefresh(win);
    return CLEAN_EXIT_WELC_SCR;
}

bool check_if_repo()
{
    git_libgit2_init();
    git_repository *cur_repo = NULL;
    int error;
    error = git_repository_open_ext(&cur_repo,".",0,NULL);
    git_repository_free(cur_repo);
    git_libgit2_shutdown();
    return error == 0;
}
int get_commit_count()
{
	int count = 0;
	git_libgit2_init();
	git_repository *repo = NULL;
	git_revwalk *walker = NULL;
	int open_error = git_repository_open_ext(&repo,".",0,NULL);
    git_revwalk_new(&walker,repo);
    git_revwalk_sorting(walker,GIT_SORT_NONE);
    git_revwalk_push_head(walker);
    git_oid commit_id;
	while(!  git_revwalk_next(&commit_id,walker) )
	    count++;
    git_repository_free(repo);
    git_libgit2_shutdown();
	return count;
}

int print_git_repo_error(WINDOW *win)
{
    if ( ! win )
        return ERR_EXIT_OPEN_ERR;

    static const char* git_dir_err_msg = "Fatal! Not a git repository!";
    static const char* bottom_msg = "Press Q to quit";
    wclear(win);
    int row,col;
    getmaxyx(win,row,col);
    ;
    wrefresh(win);

    mvwprintw(win,row/2,(col-strlen(git_dir_err_msg))/2,"%s",git_dir_err_msg);
    mvwprintw(win,row-2,(col-strlen(bottom_msg))/2,"%s",bottom_msg);
    
    wrefresh(win);

    return CLEAN_EXIT_OPEN_ERR;
}

int repo_commit_menu(WINDOW *win)
{
    if ( ! win )
        return ERR_EXIT_REPO_DET_SCR;
    int key_press,i = 0,lc = 0;
    int commit_count = get_commit_count();
    MENU *commit_details_menu;
    ITEM **menu_items;
    git_libgit2_init();
    git_repository *root_repo = NULL;
    git_revwalk *walker = NULL;
    int open_error = git_repository_open_ext(&root_repo,".",0,NULL);
    git_revwalk_new(&walker,root_repo);
    git_revwalk_sorting(walker,GIT_SORT_NONE);
    git_revwalk_push_head(walker);
    git_oid commit_id;
    wclear(win);
    ;
    menu_items = (ITEM**)calloc(commit_count+1,sizeof(ITEM*));
    while(!  git_revwalk_next(&commit_id,walker) )
    {
        git_commit *commit_obj = NULL;
        git_commit_lookup(&commit_obj,root_repo,&commit_id);
        menu_items[lc] = new_item(strdup(git_commit_summary(commit_obj)),"");
        lc++;
        git_commit_free(commit_obj);
    }
    git_repository_free(root_repo);
    git_libgit2_shutdown();
    menu_items[commit_count] = (ITEM*)NULL;
    commit_details_menu = new_menu((ITEM**)menu_items);
    post_menu(commit_details_menu);
    wrefresh(win); 
    while ( (key_press = getch()) != EXIT_KEYPRESS_CODE )
    {
        switch(key_press)
        {
            case KEY_DOWN:
                menu_driver(commit_details_menu,REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(commit_details_menu,REQ_UP_ITEM);
                break;
        }
    }
    unpost_menu(commit_details_menu);
    for(i = 0;i < commit_count;i++)
        free_item(menu_items[i]);
    
    free_menu(commit_details_menu);
    wrefresh(win);
    return CLEAN_EXIT_REPO_DET_SCR;
                
}


