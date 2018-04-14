/* Function headers for gitc
 * Author:Aditya Visvanathan
 * License:TBD
 */


#ifndef _GITC_H_
#include "gitc.h"
#endif
char *const_to_str(const char* cstr)
{
    if ( cstr )
    {
        char *str = calloc(strlen(cstr)+1,sizeof(*cstr));
        if ( ! str )
            fprintf(stdout,"calloc() failed!!");

        strcpy(str,cstr);
        return str;
    }
    else
        return NULL;
}
void print_gitc_ver()
{
    fprintf(stdout,"\ngitc version:unreleased testing channel");
}

/* body for welcome screen printing */
int print_welc_scr(WINDOW* win)
{
    
    /* check if curses window exists */
    if ( ! win )
        return E_WIN_ERR;


    int row,col,keypress;

    /* char* const to display messages */
    static const char* title_msg_top = "gitc : Git-Curses";
    static const char* des_msg_centre = "A pager for the Git Version Control System";
    static const char* fol_msg = "Press any key to continue..";
    static const char* exit_msg = "Press Q to quit";

    /* grab window details */
    getmaxyx(win,row,col);

    /* print necessary details for welcome screen */
    mvwprintw(win,1,(col-strlen(title_msg_top))/2,"%s",title_msg_top);
    mvwprintw(win,row/2,(col-strlen(des_msg_centre))/2,"%s",des_msg_centre);
    mvwprintw(win,(row/2)+2,(col-strlen(fol_msg))/2,"%s",fol_msg);
    mvwprintw(win,(row-2),(col-strlen(exit_msg))/2,"%s",exit_msg);
    wrefresh(win);

    /* grab key press as long as gitc is running */
    while ( (keypress = getch() ) != EXIT_KEY )
    {
        /* handle window resizing */                                        /* write a portable sigwinch handler */
        if ( keypress == KEY_RESIZE )
        {

            
            /* clear window and reprint all intro messages */               /* make this more resuable */
            wclear(win);
            getmaxyx(win,row,col);
            mvwprintw(win,1,(col-strlen(title_msg_top))/2,"%s",title_msg_top);
            mvwprintw(win,row/2,(col-strlen(des_msg_centre))/2,"%s",des_msg_centre);
            mvwprintw(win,(row/2)+2,(col-strlen(fol_msg))/2,"%s",fol_msg);
            mvwprintw(win,(row-2),(col-strlen(exit_msg))/2,"%s",exit_msg);
            
            /* refresh all changes to win */
            wrefresh(win);
        }
        else
            return E_SUCCESS;
    }

    return E_EXIT;
}


/* print a char* text in centre of a window */
int wprint_text_mid(WINDOW *win,char *text)
{
    if ( ! win )
        return E_WIN_ERR;
    int row,col;
    getmaxyx(win,row,col);
    mvwprintw(win,row/2,(col-strlen(text))/2,"%s",text);
    return E_SUCCESS;
}

/* check if libgit2 is able to open a git repo in current working directory */
bool check_if_repo()
{
    git_libgit2_init();
    git_repository *cur_repo = NULL;
    int error;
    error = git_repository_open_ext(&cur_repo,".",0,NULL);
    git_repository_free(cur_repo);
    git_libgit2_shutdown();
    return (error == 0);
}

/* get commit_count of a repo in cwd,-1 if no repo found */
int get_commit_count()
{
	int count = 0;
	
    /* initialize libgit2 */
    git_libgit2_init();
	git_repository *repo = NULL;
	git_revwalk *walker = NULL;

    /* open repository */
	int open_error = git_repository_open_ext(&repo,".",0,NULL);
    
    /* throw error if repo open does not succeed */
    if ( open_error != 0 )
        return E_REPO_ERR;

    /* set up git revwalk to traverse commits */
    git_revwalk_new(&walker,repo);
    git_revwalk_sorting(walker,GIT_SORT_NONE);
    git_revwalk_push_head(walker);
    git_oid commit_id;

    /* parse commit history */
	while(!  git_revwalk_next(&commit_id,walker) )
	    count++;

    /* deinitialize libgit2 stuff */
    git_repository_free(repo);
    git_libgit2_shutdown();
	return count;
}
/* print commit_menu and diff details for each window */            /* make this smaller possibly and less monolithic */
int repo_commit_menu(WINDOW *win)
{
    /* check valid window */
    if ( ! win )
           return E_WIN_ERR;

    /* variables for looping repo and storing window dimensions */
    int i = 0,lc = 0,row,col,sub_row,sub_col;
    int keypress;

    /* get window dimensions and repo commit count */
    getmaxyx(win,row,col);
    int commit_count = get_commit_count();

    /* initialize menu */
    MENU *commit_summary_menu;
    ITEM **menu_items;
    ITEM *selected_item;
    WINDOW *commit_diff_win = NULL;

    /* initialize libgit2 stuff */
    bool enter_keypressed = 0;
    git_libgit2_init();

    git_repository *root_repo = NULL;
    git_revwalk *walker = NULL;
    git_commit *parent_commit = NULL;
    git_tree *commit_tree = NULL,*parent_tree = NULL;
    git_diff *diff = NULL;
    git_buf gbuf = GIT_BUF_INIT_CONST(NULL,0);

    /* open git repository */
    int open_err = git_repository_open_ext(&root_repo,".",0,NULL);
    if (  open_err != 0 )
        return E_REPO_ERR;

    /* initialize git_revwalk to traverse through commit history */
    git_revwalk_new(&walker,root_repo);
    git_revwalk_sorting(walker,GIT_SORT_NONE);
    git_revwalk_push_head(walker);
    git_oid commit_id,sel_oid;

    /* commit,stats and time placeholder variables */
    git_commit *sel_commit = NULL;
    git_diff_stats *stats = NULL;
    time_t time;

    /* clear window */
    wclear(win);

    /* allocate n memory locations for menu items */
    menu_items = (ITEM**) calloc (commit_count + 1,sizeof (ITEM*));

    /* commit traversal and insert into menu */
    while(!  git_revwalk_next(&commit_id,walker) )
    {
        git_commit *commit_obj = NULL;
        git_commit_lookup(&commit_obj,root_repo,&commit_id);

        /* get commit summaryand commit id in char* */
        char* commit_summary_str = const_to_str(git_commit_summary(commit_obj));
        char* commit_id_str = const_to_str(git_oid_tostr_s(&commit_id));

        /* insert commit summary and commit id into menu */
        menu_items[lc] = new_item(commit_summary_str,commit_id_str);
        lc++;
        git_commit_free(commit_obj);
    }

    /* truncate last position of menu_items array  */
    menu_items[commit_count] = (ITEM*)NULL;

    /* create new menu based on items array */
    commit_summary_menu = new_menu((ITEM**)menu_items);
    
    /* set menu spacing and menu size */
    set_menu_format(commit_summary_menu,row,1);
    set_menu_spacing(commit_summary_menu,TABSIZE-4,1,0);


    /* print menu on screen with given dimensions */
    post_menu(commit_summary_menu);
    wrefresh(win); 


    /* menu key press, exit if user presses q */
    while ( (keypress = getch() ) != EXIT_KEY )
    {
        switch (keypress)
        {
            
            /* UP arrow press or VI up press */
            case VI_KEY_DOWN:
            case DOWN_KEY:
                menu_driver (commit_summary_menu,REQ_DOWN_ITEM);
                break;

            /* DOWN arrow press or VI down press */
            case VI_KEY_UP:
            case UP_KEY:
                menu_driver (commit_summary_menu,REQ_UP_ITEM);
                break;

            /* enter key press to select commit */
            case ENTER_KEY:
                if ( enter_keypressed == 0 )
                {
                    if ( commit_diff_win  == NULL ) 
                        commit_diff_win = newwin (row,col/2,col/2,0);
                    enter_keypressed = 1;
                }
                break;
            
            /* resize key press to handle window resizing */    
            case KEY_RESIZE:
                /* get new dimensions of window after resizing */
                getmaxyx (win,row,col);

                /* store currently selected item */
                selected_item = current_item (commit_summary_menu);

                /* clear menu from screen and clear the screen */
                unpost_menu (commit_summary_menu);
                wclear (win);

                /* resize commit subwindow as well if present */
                if ( enter_keypressed == 1 )
                {   
                    wresize(commit_diff_win,row,col/2);
                    mvwin(commit_diff_win,0,col/2);
                    getmaxyx(commit_diff_win,sub_row,sub_col);
                    wrefresh(commit_diff_win);
                }

                /* reset menu format (rows,columns) and print menu to screen */
                set_menu_format(commit_summary_menu,row,1);
                post_menu(commit_summary_menu);

                /* set previous current item */
                set_current_item(commit_summary_menu,selected_item);
                wrefresh(win);
                break;
            /* do nothing if anything else pressed */
            default:
                /* do nothing */
                break;

        }
        /* print commit stats if commit is selected in menu */
        if ( enter_keypressed  == 1)
        {
            selected_item = current_item(commit_summary_menu);
            if( commit_diff_win != NULL ) 
                delwin(commit_diff_win);
            commit_diff_win = newwin(row,col/2,0,col/2);
            box(commit_diff_win,0,0);
            
            /* get commit id from selected item in menu */
            const char  *oid_str = item_description(current_item(commit_summary_menu));
            git_oid_fromstr(&sel_oid,oid_str);

            /* use selected item to look up current commit,parent commit and tree */
            git_commit_lookup(&sel_commit,root_repo,&sel_oid);
            git_commit_parent(&parent_commit,sel_commit,0);
            git_commit_tree(&commit_tree,sel_commit);
            git_commit_tree(&parent_tree,parent_commit);

            /* get diff between working commit and parent commit */
            git_diff_tree_to_tree(&diff,root_repo,parent_tree,commit_tree,NULL);
            git_diff_get_stats(&stats,diff);

            /* set stats format and store in buffer */
            git_diff_stats_to_buf(&gbuf,stats,GIT_DIFF_STATS_FULL,80);
            time = git_commit_time(sel_commit);

            /* print commit diff stats  and other info */
            mvwprintw(commit_diff_win,1,1,"Commit message : %s",item_name(selected_item));
            mvwprintw(commit_diff_win,2,1,"Commit ID : %s",item_description(selected_item));
            mvwprintw(commit_diff_win,3,1,"Author : %s",git_commit_author(sel_commit)->name);
            mvwprintw(commit_diff_win,4,1,"Email : %s",git_commit_author(sel_commit)->email);
            mvwprintw(commit_diff_win,5,1,"Time : %s",ctime(&time));
            mvwprintw(commit_diff_win,7,0,"%s",gbuf.ptr);
            box(commit_diff_win,0,0);

            /* git diff cleanup */
            git_buf_free(&gbuf);
            git_diff_stats_free(stats);
            wrefresh(commit_diff_win);
            
        }
        wrefresh(win);

    }

    /* menu items cleanup */
    unpost_menu(commit_summary_menu);
    for (i = 0; i < commit_count; i++ )
        free_item ( menu_items[i] );
    
    /* menu cleanup */
    free_menu( commit_summary_menu );
    wrefresh(win);
    delwin(commit_diff_win);

    /* libgit2 cleanup */
    git_commit_free(sel_commit);
    git_repository_free(root_repo);
    git_libgit2_shutdown();
    return E_SUCCESS;
                
}


