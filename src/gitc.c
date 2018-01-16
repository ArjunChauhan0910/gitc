/* Function headers for gitc
 * Author:Aditya Visvanathan
 * License:TBD
 */

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <git2.h>
#include <menu.h>
#include "gitc.h"

int print_welc_scr(WINDOW* win)
{
    if ( ! win )
        return E_EXIT;
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
    while ( (keypress = getch() ) != EXIT_KEY )
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
            return E_EXIT;
    }

    return E_SUCCESS;
}

int wprint_text_mid(WINDOW *win,char *text)
{
    if ( ! win )
        return E_EXIT;
    int row,col;
    getmaxyx(win,row,col);
    mvwprintw(win,row/2,(col-strlen(text))/2,"%s",text);
    return E_SUCCESS;
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

char *get_committer_name_from_oid(git_oid oid,git_repository *repo)
{
    git_commit *commit = NULL;
    git_commit_lookup(&commit,repo,&oid);
    return strdup(git_commit_committer(commit)->name);
}

int repo_commit_menu(WINDOW *win)
{
    if ( ! win )
        return -1;
    int i = 0,lc = 0,row,col,sub_row,sub_col;
    keybind keypress;
    getmaxyx(win,row,col);
    int commit_count = get_commit_count();
    MENU *commit_summary_menu;
    ITEM **menu_items;
    ITEM *selected_item;
    WINDOW *commit_diff_win = NULL;
    bool enter_keypressed = 0;
    git_libgit2_init();
    git_repository *root_repo = NULL;
    git_revwalk *walker = NULL;
    git_commit *parent_commit = NULL;
    git_tree *commit_tree = NULL,*parent_tree = NULL;
    git_diff *diff = NULL;
    git_buf gbuf = GIT_BUF_INIT_CONST(NULL,0);
    int open_error = git_repository_open_ext(&root_repo,".",0,NULL);
    git_revwalk_new(&walker,root_repo);
    git_revwalk_sorting(walker,GIT_SORT_NONE);
    git_revwalk_push_head(walker);
    git_oid commit_id,sel_oid;
    git_commit *sel_commit = NULL;
    git_diff_stats *stats = NULL;
    time_t time;
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
    menu_items[commit_count] = (ITEM*)NULL;
    commit_summary_menu = new_menu((ITEM**)menu_items);
    set_menu_format(commit_summary_menu,row,1);
    set_menu_spacing(commit_summary_menu,TABSIZE-4,1,0);
    post_menu(commit_summary_menu);
    wrefresh(win); 
    while ( (keypress = getch() ) != EXIT_KEY )
    {
        switch(keypress)
        {
            case VI_KEY_DOWN:
            case DOWN_KEY:
                menu_driver(commit_summary_menu,REQ_DOWN_ITEM);
                break;
            case VI_KEY_UP:
            case UP_KEY:
                menu_driver(commit_summary_menu,REQ_UP_ITEM);
                break;
            case ENTER_KEY:
                if ( enter_keypressed == 0 )
                {
                    commit_diff_win = newwin(row,col/2,0,col/2);
                    enter_keypressed = 1;
                }
                break;
                
            case KEY_RESIZE:
                getmaxyx(win,row,col);
                selected_item = current_item(commit_summary_menu);
                unpost_menu(commit_summary_menu);
                wclear(win);
                if ( enter_keypressed == 1 )
                {   
                    wresize(commit_diff_win,row,col/2);
                    mvwin(commit_diff_win,0,col/2);
                    getmaxyx(commit_diff_win,sub_row,sub_col);
                    wrefresh(commit_diff_win);
                }
                set_menu_format(commit_summary_menu,row,1);
                post_menu(commit_summary_menu);
                set_current_item(commit_summary_menu,selected_item);
                wrefresh(win);
                break;
            default:
                /* do nothing */
                break;

        }
        if ( enter_keypressed  == 1)
        {
            selected_item = current_item(commit_summary_menu);
            if( commit_diff_win != NULL ) 
                delwin(commit_diff_win);
            commit_diff_win = newwin(row,col/2,0,col/2);
            box(commit_diff_win,0,0);
            char *oid_str = strdup(item_description(current_item(commit_summary_menu)));
            git_oid_fromstr(&sel_oid,oid_str);
            git_commit_lookup(&sel_commit,root_repo,&sel_oid);
            git_commit_parent(&parent_commit,sel_commit,0);
            git_commit_tree(&commit_tree,sel_commit);
            git_commit_tree(&parent_tree,parent_commit);
            git_diff_tree_to_tree(&diff,root_repo,parent_tree,commit_tree,NULL);
            git_diff_get_stats(&stats,diff);
            git_diff_stats_to_buf(&gbuf,stats,GIT_DIFF_STATS_FULL,80);
            time = git_commit_time(sel_commit);
            mvwprintw(commit_diff_win,1,1,"Commit message : %s",item_name(selected_item));
            mvwprintw(commit_diff_win,2,1,"Commit ID : %s",item_description(selected_item));
            mvwprintw(commit_diff_win,3,1,"Author : %s",strdup(git_commit_author(sel_commit)->name));
            mvwprintw(commit_diff_win,4,1,"Email : %s",strdup(git_commit_author(sel_commit)->email));
            mvwprintw(commit_diff_win,5,1,"Time : %s",ctime(&time));
            mvwprintw(commit_diff_win,7,0,gbuf.ptr);
            box(commit_diff_win,0,0);
            git_buf_free(&gbuf);
            git_diff_stats_free(stats);
            wrefresh(commit_diff_win);
            
        }
        wrefresh(win);

    }
    unpost_menu(commit_summary_menu);
    for(i = 0;i < commit_count;i++)
        free_item(menu_items[i]);
    
    free_menu(commit_summary_menu);
    wrefresh(win);
    delwin(commit_diff_win);
    git_commit_free(sel_commit);
    git_repository_free(root_repo);
    git_libgit2_shutdown();
    return E_SUCCESS;
                
}


