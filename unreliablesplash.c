#define _XOPEN_SOURCE_EXTENDED 1

//gcc unreliablesplash.c llist.c -lncursesw && a.out

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ncursesw/curses.h>
#include <locale.h>
#include <wchar.h>

#include "llist.h"

#define SYSCALL_THREADNUM 0

typedef struct uimessage_t
{
  char* username;
  int userid;
  char** message;
  int numlines;
} uimessage_t;

typedef struct window_t
{
  WINDOW* window;
  int r;
  int c;
  int nrows;
  int ncols;
  int hasfocus;
} window_t;

llist_t* MSGS;
llist_t* INFOS;

char MY_MSG[1024];
int MY_MSG_INDEX;

int r;
int c;
int nrows;
int ncols;
WINDOW* mainwnd;
window_t* splashwnd;
window_t* inputwnd;
window_t* infownd;
window_t* msgwnd;

window_t* focuswnd;

window_t* focusable_wnds[3];
int focusindex;

void splash()
{
  printf("\E[H\E[2J");
  printf("\n");
  printf("\E[1m\E[34m  $$$$  $$$$                                  $$$$ $$$$            $$$$       $$$$\E(B\E[m\n");
  printf("\E[1m\E[34m  $$$$  $$$$                                  $$$$ ****            $$$$       $$$$\E(B\E[m\n");
  printf("\E[1m\E[34m  $$$$  $$$$ $$$o$$$o.  $$$o$$$o.   .o$$$$o.  $$$$ $$$$  .o$$$$$o. $$$$o$$o.  $$$$  .o$$$$o.\E(B\E[m\n");
  printf("\E[1m\E[34m  $$$$  $$$$ $$$$  $$$. $$$$  $$$o .$$$  $$$o $$$$ $$$$  ***  $$$$ $$$$  $$$. $$$$ .$$$  $$$o\E(B\E[m\n");
  printf("\E[1m\E[34m  $$$$  $$$$ $$$$  $$$$ $$$$  $$$$ $$$$$$$$$$ $$$$ $$$$  o$$$$$$$$ $$$$  $$$$ $$$$ $$$$$$$$$$\E(B\E[m\n");
  printf("\E[1m\E[34m  $$$$  $$$$ $$$$  $$$$ $$$$       *$$$  oooo $$$$ $$$$ $$$$  $$$$ $$$$  $$$* $$$$ *$$$  oooo\E(B\E[m\n");
  printf("\E[1m\E[34m  '*$$$$*$$$ $$$$  $$$$ $$$$        '*$$$$$*' $$$$ $$$$  *$$$$*$$$ $$$*$$$*'  $$$$  '*$$$$$*'\E(B\E[m\n");
  printf("\E[1m\E(B\E[m\n");
  printf("\E[1m\E[36m   .o$$$$o.  $$$$                          \E[37mo$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$o\E(B\E[m\n");
  printf("\E[1m\E[36m  o$$$  $$$o $$$$                  $$$$    \E[37m$$                                              \E[37m$$\E(B\E[m\n");
  printf("\E[1m\E[36m  $$$$  $$$$ $$$$$$$o.   .o$$$$$o. $$$$$$  \E[37m$$       \E[34mくコBミ  \E[36mくコ:彡       \E[33m\E[5mHow so Blur?\E[25m    \E[37m$$\E(B\E[m\n");
  printf("\E[1m\E[36m  $$$$       $$$$  $$$o  ***  $$$$ $$$$    \E[37m$$  \E[36mくコ8彡           \E[34mくコ8ミ  \E[36mくコ:彡          \E[37m$$\E(B\E[m\n");
  printf("\E[1m\E[36m  $$$$  $$$$ $$$$  $$$$  o$$$$$$$$ $$$$    \E[37m$$               \E[33m\E[5mHow so Blur?\E[25m          \E[34mくコ8ミ  \E[37m$$\E(B\E[m\n");
  printf("\E[1m\E[36m  *$$$  $$$* $$$$  $$$$ $$$$  $$$$ $$$$    \E[37m$$    \E[34mくコB彡    \E[34mくコBミ         \E[36mくコ8彡        \E[37m$$\E(B\E[m\n");
  printf("\E[1m\E[36m   '*$$$$*'  $$$$  $$$$  *$$$$*$$$ '*$$$$  \E[37m$$            \E[36mくコ8彡  \E[33m\E[5mHow so Blur?\E[25m \E[34mくコ:ミ     \E[37m$$\E(B\E[m\n");
  printf("\E[1m                                           \E[37m$$ \E[36mくコ:彡           \E[34mくコ8ミ      \E[36mくコB彡       \E[37m$$\E(B\E[m\n");
  printf("\E[1m\E[37m  $$$$$$$$$$  \E[34mくコ8彡 \E[37m#1  \E[25m\E[33mSpencer Caplan\E[25m   \E[37m$$       \E[33m\E[5mHow so Blur?\E[25m     \E[36mくコBミ       \E[34mくコ8彡 \E[37m$$\E(B\E[m\n");
  printf("\E[1m\E[37m  $$$$  $$$$  \E[36mくコ:ミ \E[37m#2  \E[25m\E[33mJordan~ Kodner\E[25m   \E[37m$$                                              \E[37m$$\E(B\E[m\n");
  printf("\E[1m\E[37m  $$$$$$$$$$  \E[34mくコB彡 \E[37m#3  \E[25m\E[33mAnupama Kumar~\E[25m   \E[37m*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*\E(B\E[m\n");
  printf("\n");
}

void splash2()
{
  printf("\n");
  printf("\E[1m\E[34m  $$$$  $$$$                                  $$$$ $$$$            $$$$       $$$$\E(B\E[m\n");
  printf("\E[1m\E[34m  $$$$  $$$$                                  $$$$ ****            $$$$       $$$$\E(B\E[m\n");
  printf("\E[1m\E[34m  $$$$  $$$$ $$$o$$$o.  $$$o$$$o.   .o$$$$o.  $$$$ $$$$  .o$$$$$o. $$$$o$$o.  $$$$  .o$$$$o.\E(B\E[m\n");
  printf("\E[1m\E[34m  $$$$  $$$$ $$$$  $$$. $$$$  $$$o .$$$  $$$o $$$$ $$$$  ***  $$$$ $$$$  $$$. $$$$ .$$$  $$$o\E(B\E[m\n");
  printf("\E[1m\E[34m  $$$$  $$$$ $$$$  $$$$ $$$$  $$$$ $$$$$$$$$$ $$$$ $$$$  o$$$$$$$$ $$$$  $$$$ $$$$ $$$$$$$$$$\E(B\E[m\n");
  printf("\E[1m\E[34m  $$$$  $$$$ $$$$  $$$$ $$$$       *$$$  oooo $$$$ $$$$ $$$$  $$$$ $$$$  $$$* $$$$ *$$$  oooo\E(B\E[m\n");
  printf("\E[1m\E[34m  '*$$$$*$$$ $$$$  $$$$ $$$$        '*$$$$$*' $$$$ $$$$  *$$$$*$$$ $$$*$$$*'  $$$$  '*$$$$$*'\E(B\E[m\n");
  printf("\E[1m\E(B\E[m\n");
  printf("\E[1m\E[36m   .o$$$$o.  $$$$                          \E[37mo$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$o\E(B\E[m\n");
  printf("\E[1m\E[36m  o$$$  $$$o $$$$                  $$$$    \E[37m$$                                              \E[37m$$\E(B\E[m\n");
  printf("\E[1m\E[36m  $$$$  $$$$ $$$$$$$o.   .o$$$$$o. $$$$$$  \E[37m$$         \E[34mくコB彡  \E[36mくコ:ミ     \E[33m\E[5mHow so Blur?\E[25m    \E[37m$$\E(B\E[m\n");
  printf("\E[1m\E[36m  $$$$       $$$$  $$$o  ***  $$$$ $$$$    \E[37m$$    \E[36mくコ8ミ       \E[34mくコ8彡  \E[36mくコ:ミ            \E[37m$$\E(B\E[m\n");
  printf("\E[1m\E[36m  $$$$  $$$$ $$$$  $$$$  o$$$$$$$$ $$$$    \E[37m$$               \E[33m\E[5mHow so Blur?\E[25m        \E[34mくコ8彡    \E[37m$$\E(B\E[m\n");
  printf("\E[1m\E[36m  *$$$  $$$* $$$$  $$$$ $$$$  $$$$ $$$$    \E[37m$$  \E[34mくコBミ        \E[34mくコB彡         \E[36mくコ8ミ      \E[37m$$\E(B\E[m\n");
  printf("\E[1m\E[36m   '*$$$$*'  $$$$  $$$$  *$$$$*$$$ '*$$$$  \E[37m$$          \E[36mくコ8ミ    \E[33m\E[5mHow so Blur?\E[25m   \E[34mくコ:彡   \E[37m$$\E(B\E[m\n");
  printf("\E[1m                                           \E[37m$$ \E[36m  くコ:ミ           \E[34mくコ8彡  \E[36mくコBミ         \E[37m$$\E(B\E[m\n");
  printf("\E[1m\E[37m  $$$$$$$$$$  \E[34mくコ8ミ \E[37m#1  \E[25m\E[33mSpencer Caplan\E[25m   \E[37m$$       \E[33m\E[5mHow so Blur?\E[25m       \E[36mくコB彡   \E[34mくコ8ミ   \E[37m$$\E(B\E[m\n");
  printf("\E[1m\E[37m  $$$$  $$$$  \E[36mくコ:彡 \E[37m#2  \E[25m\E[33mJordan~ Kodner\E[25m   \E[37m$$                                              \E[37m$$\E(B\E[m\n");
  printf("\E[1m\E[37m  $$$$$$$$$$  \E[34mくコBミ \E[37m#3  \E[25m\E[33mAnupama Kumar~\E[25m   \E[37m*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*\E(B\E[m\n");
  printf("\n");
}


void splashcurses()
{
  init_pair(1, COLOR_BLUE, COLOR_BLACK);
  init_pair(2, COLOR_CYAN, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_WHITE, COLOR_BLACK);
  wattron(splashwnd->window,COLOR_PAIR(1));
  wattron(splashwnd->window,A_BOLD);
  //Unreliable
  mvwaddwstr(splashwnd->window,2,1,L"  $$$$  $$$$                                  $$$$ $$$$            $$$$       $$$$");
  mvwaddwstr(splashwnd->window,3,1,L"  $$$$  $$$$                                  $$$$ ****            $$$$       $$$$");
  mvwaddwstr(splashwnd->window,4,1,L"  $$$$  $$$$ $$$o$$$o.  $$$o$$$o.   .o$$$$o.  $$$$ $$$$  .o$$$$$o. $$$$o$$o.  $$$$  .o$$$$o.");
  mvwaddwstr(splashwnd->window,5,1,L"  $$$$  $$$$ $$$$  $$$o $$$$  $$$o .$$$  $$$o $$$$ $$$$  ***  $$$$ $$$$  $$$. $$$$ .$$$  $$$o");
  mvwaddwstr(splashwnd->window,6,1,L"  $$$$  $$$$ $$$$  $$$$ $$$$  $$$$ $$$$$$$$$$ $$$$ $$$$  o$$$$$$$$ $$$$  $$$$ $$$$ $$$$$$$$$$");
  mvwaddwstr(splashwnd->window,7,1,L"  $$$$  $$$$ $$$$  $$$$ $$$$       *$$$  oooo $$$$ $$$$ $$$$  $$$$ $$$$  $$$* $$$$ *$$$  oooo");
  mvwaddwstr(splashwnd->window,8,1,L"  '*$$$$*$$$ $$$$  $$$$ $$$$        '*$$$$$*' $$$$ $$$$  *$$$$*$$$ $$$*$$$*'  $$$$  '*$$$$$*'");
  wattroff(splashwnd->window,COLOR_PAIR(1));
  wattron(splashwnd->window,COLOR_PAIR(2));
  //Chat
  mvwaddwstr(splashwnd->window,10,1,L"   .o$$$$o.  $$$$");
  mvwaddwstr(splashwnd->window,11,1,L"  o$$$  $$$o $$$$                  $$$$");
  mvwaddwstr(splashwnd->window,12,1,L"  $$$$  $$$$ $$$$$$$o.   .o$$$$$o. $$$$$$");
  mvwaddwstr(splashwnd->window,13,1,L"  $$$$       $$$$  $$$o  ***  $$$$ $$$$");
  mvwaddwstr(splashwnd->window,14,1,L"  $$$$  $$$$ $$$$  $$$$  o$$$$$$$$ $$$$");
  mvwaddwstr(splashwnd->window,15,1,L"  *$$$  $$$* $$$$  $$$$ $$$$  $$$$ $$$$");
  mvwaddwstr(splashwnd->window,16,1,L"   '*$$$$*'  $$$$  $$$$  *$$$$*$$$ '*$$$$");
  wattroff(splashwnd->window,COLOR_PAIR(2));

  //credits
  wattron(splashwnd->window,COLOR_PAIR(4));
  mvwaddwstr(splashwnd->window,18,1,L"  $$$$$$$$$$");
  mvwaddwstr(splashwnd->window,19,1,L"  $$$$  $$$$");
  mvwaddwstr(splashwnd->window,20,1,L"  $$$$$$$$$$");
  mvwaddwstr(splashwnd->window,18,19,L" #1");
  mvwaddwstr(splashwnd->window,19,19,L" #2");
  mvwaddwstr(splashwnd->window,20,19,L" #3");
  wattroff(splashwnd->window,COLOR_PAIR(4));
  wattron(splashwnd->window,COLOR_PAIR(1));
  mvwaddwstr(splashwnd->window,20,15,L"くコB彡");
  mvwaddwstr(splashwnd->window,18,15,L"くコ8彡");
  wattroff(splashwnd->window,COLOR_PAIR(1));
  wattron(splashwnd->window,COLOR_PAIR(2));
  mvwaddwstr(splashwnd->window,19,15,L"くコ:ミ");
  wattroff(splashwnd->window,COLOR_PAIR(2));
  wattron(splashwnd->window,COLOR_PAIR(3));
  mvwaddwstr(splashwnd->window,18,23,L"Spencer Caplan");
  mvwaddwstr(splashwnd->window,19,23,L"Jordan~ Kodner");
  mvwaddwstr(splashwnd->window,20,23,L"Anupama Kumar~");
  wattroff(splashwnd->window,COLOR_PAIR(3));

  //sotong tank
  mvwaddwstr(splashwnd->window,10,44,L"o$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$o");
  mvwaddwstr(splashwnd->window,11,44,L"$$                                              $$");
  mvwaddwstr(splashwnd->window,12,44,L"$$       くコBミ  くコ:彡       How so Blur?    $$");
  mvwaddwstr(splashwnd->window,13,44,L"$$  くコ8彡           くコ8ミ  くコ:彡          $$");
  mvwaddwstr(splashwnd->window,14,44,L"$$               How so Blur?          くコ8ミ  $$");
  mvwaddwstr(splashwnd->window,15,44,L"$$    くコB彡    くコBミ         くコ8彡        $$");
  mvwaddwstr(splashwnd->window,16,44,L"$$            くコ8彡  How so Blur? くコ:ミ     $$");
  mvwaddwstr(splashwnd->window,17,44,L"$$ くコ:彡           くコ8ミ      くコB彡       $$");
  mvwaddwstr(splashwnd->window,18,44,L"$$       How so Blur?     くコBミ       くコ8彡 $$");
  mvwaddwstr(splashwnd->window,19,44,L"$$                                              $$");
  mvwaddwstr(splashwnd->window,20,44,L"*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*");


  wattroff(splashwnd->window,A_BOLD);
  wrefresh(splashwnd->window);
}



void add_msg(char* user, char message[])
{
  int maxline = msgwnd->ncols/2-2;
  int numlines = strlen(message)/maxline;
  if(strlen(message)%maxline > 0)
    numlines++;
  char** msglines = (char**)malloc(sizeof(char)*numlines*maxline);
  int lo = 0;
  int hi = 0+maxline;
  int index = 0;

  while(lo+maxline < strlen(message))
  {
    msglines[index] = (char*)malloc(sizeof(char)*maxline+1);
    strncpy(msglines[index],&message[lo],lo+maxline);
    msglines[index][maxline] = '\0';
    lo += maxline;
    index++;
  }
  msglines[index] = (char*)malloc(sizeof(char)*maxline);
  strncpy(msglines[index],&message[lo],strlen(&message[lo]));
  uimessage_t* newmessage = (uimessage_t*)malloc(sizeof(uimessage_t));
  newmessage->username = user;
  newmessage->userid = 1;
  newmessage->message = msglines;
  newmessage->numlines = numlines;
  add_elem(MSGS, (void*)newmessage);

  int i = 0;
  for(i = numlines-1; i >= 0; i--)
  {
    //    mvwaddstr(msgwnd->window,msgwnd->nrows-2-i, 38, "hello");
    mvwaddstr(msgwnd->window,msgwnd->nrows-2+i-numlines, 38, msglines[i]);
  }
  wrefresh(msgwnd->window);
}

void print_msgs()
{
  int linenum = msgwnd->nrows-2;

  node_t* curr = MSGS->tail;
  wclear(msgwnd->window);
  setborder(msgwnd);
  while(curr != NULL)
  {
    uimessage_t* uimsg = (uimessage_t*)(curr->elem);
    int start = 10;
    int namestart = 2;
    if(strcmp("Me", uimsg->username) == 0)
    {
      start = msgwnd->ncols/2;
      namestart = msgwnd->ncols-2-strlen(uimsg->username);
    }
    int i = 0;
    for(i = uimsg->numlines-1; i >= 0; i--)
    {
      //    mvwaddstr(msgwnd->window,msgwnd->nrows-2-i, 38, "hello");
      mvwaddstr(msgwnd->window,linenum, start, uimsg->message[i]);
      linenum--;
      if(linenum == 0)
	break;
    }
    wattron(msgwnd->window,COLOR_PAIR(1));    
    if(curr->prev != NULL)
    {
      if(strcmp(((uimessage_t*)curr->prev->elem)->username, uimsg->username))
	{
	mvwaddstr(msgwnd->window,linenum, namestart, uimsg->username);
	linenum--;
	}
    }
    else
    {
      mvwaddstr(msgwnd->window,linenum, namestart, uimsg->username);
      linenum--;
    }
    wattroff(msgwnd->window,COLOR_PAIR(1));    

    curr=curr->prev;
  }
    wrefresh(msgwnd->window);
}

void print_msg(char* user, char message[])
{
  add_msg(user,message);
  print_msgs();
}

void *spoof_chats(void *t)
{
  while(1)
  {
    print_msg("Alice", "Hi, how are you doing? I'm fine personally, but I'm just a function parameter, so I don't really know what I'm talking about. You know what I mean?");
    sleep(5);
    print_msg("Bob","Am Bob.");
    print_msg("Bob","Boon? Me ah?.");
    sleep(2);
  }
  pthread_exit((void *)t);
}


void draw(char dc)
{
  wmove(focuswnd->window,focuswnd->r,focuswnd->c);
  if(dc == '\n')
  {
    //    wprintw(msgwnd->window,"\n\n   ");
    //    wprintw(msgwnd->window,MY_MSG);
    //    wrefresh(msgwnd->window);
    print_msg("Me", MY_MSG);
    refresh_wnd(focuswnd);
    memset(MY_MSG,0, 1024);
    MY_MSG_INDEX = 0;
    return;
  }
  if(dc == '\b')
  {
    waddch(focuswnd->window,' ');

    if(focuswnd->c < 3)
    {

      if(focuswnd->r > 2)
      {
	focuswnd->r--;
	focuswnd->c = focuswnd->ncols-3;
      }
    }
    else
      focuswnd->c--;
    wmove(focuswnd->window, focuswnd->r, focuswnd->c);
    if(MY_MSG_INDEX > 0)
      MY_MSG_INDEX--;
    MY_MSG[MY_MSG_INDEX] = '\0';

    return;
  }

  if(focuswnd->c == focuswnd->ncols-2 && focuswnd->r == focuswnd->nrows-2)
    return;

  wdelch(focuswnd->window);
  winsch(focuswnd->window,dc);

  MY_MSG[MY_MSG_INDEX] = dc;
  MY_MSG_INDEX++;
  //  wprintw(msgwnd->window,"\n\n   ");
  //  wprintw(msgwnd->window,MY_MSG);
  //  wrefresh(msgwnd->window);

  focuswnd->c++;
  if(focuswnd->c == focuswnd->ncols-2)
  {
    focuswnd->c = 2;
    focuswnd->r++;
    if (focuswnd->r== focuswnd->nrows-2)
      focuswnd->r = 2;
  }
  wmove(focuswnd->window,focuswnd->r,focuswnd->c);
}



void setborder(window_t* wnd)
{
  int color = 10;
  if(wnd->hasfocus == 1)
  {
    if(wnd == msgwnd)
      color = 11;
    else if(wnd == inputwnd)
      color = 12;
    else
      color = 13;
  }
  //  if(wnd)
  //  draw(wnd->hasfocus+48);
  wattron(wnd->window,COLOR_PAIR(color));
  wborder(wnd->window, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wattroff(wnd->window,COLOR_PAIR(color));
  wrefresh(wnd->window);
}

void refresh_wnd(window_t* wnd)
{
  wclear(wnd->window);
  wnd->c = 2;
  wnd->r = 2;
  setborder(wnd);
  wmove(focuswnd->window,focuswnd->r,focuswnd->c);
  wrefresh(wnd->window);
}


window_t* init_wnd(int h, int w, int startr, int startc)
{
  window_t* newwnd = (window_t*)malloc(sizeof(window_t));;
  WINDOW* newwindow;

  newwindow = newwin(h,w,startr,startc);
  newwnd->window = newwindow;
  newwnd->r = 2;
  newwnd->c = 2;
  newwnd->ncols = w;
  newwnd->nrows = h;
  newwnd->hasfocus = 0;
  setborder(newwnd);
  wrefresh(newwnd->window);
  return newwnd;
}

void del_wnd(window_t* wnd)
{
  delwin(wnd->window);
}

void setfocus(window_t* newfocuswnd)
{
  focuswnd->hasfocus = 0;
  newfocuswnd->hasfocus = 1;
  setborder(focuswnd);
  setborder(newfocuswnd);
  focuswnd = newfocuswnd;
}

void nextfocus()
{
  focusindex++;
  if(focusindex > 2)
    focusindex = 0;
  setfocus(focusable_wnds[focusindex]);
}

void initui(int isdebug)
{
  char d;

  if(isdebug)
    return;

  //  MY_MSG = (char*)malloc(sizeof(char));
  MY_MSG_INDEX = 0;
  MSGS = (llist_t*)malloc(sizeof(llist_t));
  init_list(MSGS);

  //  splash();
  mainwnd = initscr();
  setlocale(LC_ALL, "en_US.utf8");
  start_color();

  init_pair(11, COLOR_BLACK, COLOR_CYAN);
  init_pair(12, COLOR_BLACK, COLOR_YELLOW);
  init_pair(13, COLOR_BLACK, COLOR_BLUE);
  init_pair(10, COLOR_BLACK, COLOR_WHITE);

  cbreak();
  noecho();
  getmaxyx(mainwnd,nrows,ncols);
  //  init_pair(10, COLOR_BLACK, COLOR_WHITE);
  //  wattron(mainwnd,COLOR_PAIR(10));
  //  wborder(mainwnd, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  //  wattroff(mainwnd,COLOR_PAIR(10));
  //  wrefresh(mainwnd);
  //  wprintw(mainwnd,"%d, %d\n",nrows,ncols);
  //  if(has_colors() == FALSE)
  //    {endwin();
  //      printf("Your terminal does not support color. Run in non-UI mode instead.\n");
  //      exit(1);
  //    }
  clear();
  //  wprintw(mainwnd,"%d, %d\n",nrows,ncols);
  wrefresh(mainwnd);

  splashwnd = init_wnd(23, 97, 0, 1);
  infownd = init_wnd(26, 97, 22, 1);
  inputwnd = init_wnd(11, 97, 47, 1);
  msgwnd = init_wnd(nrows, ncols-99, 0, 98);
  
  focusable_wnds[0] = infownd;
  focusable_wnds[1] = inputwnd;
  focusable_wnds[2] = msgwnd;
  focusindex = 1;
  focuswnd = focusable_wnds[1];
  setfocus(inputwnd);

  splashcurses();

  pthread_t threads[2];
  pthread_attr_t attr;
  void *exitstatus;

  //make sure threads are joinable
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  pthread_create(&threads[SYSCALL_THREADNUM], &attr, spoof_chats, (void *)SYSCALL_THREADNUM);

  //  pthread_join(threads[SYSCALL_THREADNUM], &exitstatus);


  while(1)
  {
    d = wgetch(focuswnd->window);
    if(d == '\t')
   {
     nextfocus();
   }
    else
      draw(d);
    }
  
}

int main(int argc, char** argv)
{
  int i = 0;

  char d;

  initui(0);

  //  del_wnd(splashwnd);
  //  del_wnd(infownd);
  //  del_wnd(inputwnd);
  //  del_wnd(msgwnd);
  endwin();

  printf("\E[H\E[2J");
  for(i = 0; i < 3; i++)
  {
    if(i % 2 == 0)
    {
      //        splash();
	//	printf("\E8");
    }
    else
    {
      //      splash2();
      //      printf("\E8");
    }
    usleep(500000);
    //printf("\E[1G\E[25A");
    printf("\E[25A");
  }
  //  splash();


  //  clear();
  //  move(50,50);


  //  struct winsize sz;
  //  ioctl(0, TIOCGWINSZ, &sz);
  //  printf("width: %i\theight: %i\n", sz.ws_col, sz.ws_row);

  return 0;
}
