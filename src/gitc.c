/* Function headers for gitc
 * Author:Aditya Visvanathan
 * License:TBD
 */
#define VER "Version"
#define EXIT_KEYPRESS_CODE 113
#define BOTTOM_WINDOW_OFFSET(a) (a - 10)
#define VI_KEY_UP 107
#define VI_KEY_DOWN 106
#define BOTTOM_MENU_OFFSET(a) (a - 10)

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
        return -1;
    int row,col,keypress;
    static const char* title_msg_top = "gitc : Git-Curses";
    static const char* des_msg_centre = "A TUI frontend for the Git Version Control System";
    static const char* fol_msg = "Press any key to continue..";
    static const char* exit_msg = "Press Q to quit";
    getmaxyx(win,row,col);
    mvwprintw(win,1,(col-strlen(title_msg_top))/2,"%s",title_msg_top);
    mvwprintw(win,row/2,(col-strlen(des_msg_centre))/2,"%s",des_msg_centre);
    mvwprintw(win,(row/2)+2,(col-strlen(fol_msg))/2,"%s",fol_msg);
    mvwprintw(win,(row-2),(col-strlen(exit_msg))/2,"%s",exit_msg);
    wrefresh(win);
    while ( (keypress = getch() ) != EXIT_KEYPRESS_CODE )
    {
        if ( keypress == KEY_RESIZE )
        {

            wclear(win);
            getmaxyx(win,row,col);
            mvwprintw(win,1,(col-strlen(title_msg_top))/2,"%s",title_msg_top);
            mvwprintw(win,row/2,(col-strlen(des_msg_centre))/2,"%s",des_msg_centre);
            mvwprintw(win,(row/2)+2,(col-strlen(fol_msg))/2,"%s",fol_msg);
            mvwprintw(win,(row-2),(col-strlen(exit_msg))/2,"%s",exit_msg);
            wrefresh(win);
        }
        else
            return 1;
    }

    return 0;
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


int repo_commit_menu(WINDOW *win)
{
    if ( ! win )
        return -1;
    int keypress,i = 0,lc = 0,row,col,sub_row,sub_col;
    getmaxyx(win,row,col);
    int commit_count = get_commit_count();
    MENU *commit_summary_menu;
    ITEM **menu_items;
    ITEM *selected_item;
    WINDOW *commit_diff_win;
    commit_diff_win = newwin(10,col,BOTTOM_MENU_OFFSET(row),0);
    getmaxyx(commit_diff_win,sub_row,sub_col);
    git_libgit2_init();
    git_repository *root_repo = NULL;
    git_revwalk *walker = NULL;
    int open_error = git_repository_open_ext(&root_repo,".",0,NULL);
    git_revwalk_new(&walker,root_repo);
    git_revwalk_sorting(walker,GIT_SORT_NONE);
    git_revwalk_push_head(walker);
    git_oid commit_id,commit_diff_oid;
    wclear(win);

    menu_items = (ITEM**)calloc(commit_count+1,sizeof(ITEM*));
    while(!  git_revwalk_next(&commit_id,walker) )
    {
        git_commit *commit_obj = NULL;
        git_commit_lookup(&commit_obj,root_repo,&commit_id);
        char* commit_summary_str = strdup(git_commit_summary(commit_obj));
        char* commit_id_str = strdup(git_oid_tostr_s(&commit_id));
        menu_items[lc] = new_item(commit_summary_str,commit_id_str);
        lc++;
        git_commit_free(commit_obj);
    }
    git_repository_free(root_repo);
    git_libgit2_shutdown();
    menu_items[commit_count] = (ITEM*)NULL;
    commit_summary_menu = new_menu((ITEM**)menu_items);
    set_menu_format(commit_summary_menu,BOTTOM_MENU_OFFSET(row),1);
    set_menu_spacing(commit_summary_menu,TABSIZE-1,1,0);
    post_menu(commit_summary_menu);
    wrefresh(win); 
    box(commit_diff_win,0,0);
    wrefresh(commit_diff_win);
    wrefresh(win);
    getmaxyx(commit_diff_win,sub_row,sub_col); 
    while ( (keypress = getch() ) != EXIT_KEYPRESS_CODE )
    {
        switch(keypress)
        {
            case VI_KEY_DOWN:
            case KEY_DOWN:
                menu_driver(commit_summary_menu,REQ_DOWN_ITEM);
                break;
            case VI_KEY_UP:
            case KEY_UP:
                menu_driver(commit_summary_menu,REQ_UP_ITEM);
                break;
            case KEY_RESIZE:
                getmaxyx(win,row,col);
                selected_item = current_item(commit_summary_menu);
                unpost_menu(commit_summary_menu);
                wclear(win);
                wresize(commit_diff_win,10,col);
                mvwin(commit_diff_win,BOTTOM_MENU_OFFSET(row),0);
                getmaxyx(commit_diff_win,sub_row,sub_col);
                set_menu_format(commit_summary_menu,BOTTOM_MENU_OFFSET(row),1);
                post_menu(commit_summary_menu);
                set_current_item(commit_summary_menu,selected_item);
                char *subtext = strdup(item_name(selected_item));
                mvwprintw(commit_diff_win,sub_row/2,(sub_col-strlen(subtext))/2,"%s",subtext);
                wrefresh(win);
                wrefresh(commit_diff_win);
                
                break;
        }
        char *text = strdup(item_name(current_item(commit_summary_menu)));
        delwin(commit_diff_win);
        commit_diff_win = newwin(10,col,BOTTOM_MENU_OFFSET(row),0);
        box(commit_diff_win,0,0);
        mvwprintw(commit_diff_win,sub_row/2,(sub_col-strlen(text))/2,"%s",text);
        wrefresh(commit_diff_win);

    }
    unpost_menu(commit_summary_menu);
    for(i = 0;i < commit_count;i++)
        free_item(menu_items[i]);
    
    free_menu(commit_summary_menu);
    wrefresh(win);
    delwin(commit_diff_win);
    return 0;
                
}


