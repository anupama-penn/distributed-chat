#pragma once 
#define _XOPEN_SOURCE_EXTENDED 1

//gcc -pthread unreliablesplash.c llist.c -lncursesw && a.out

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
#include "send_msg.h"

#define SYSCALL_THREADNUM 0

typedef struct uimessage_t
{
  char* username;
  char* hostname;
  int portnum;
  char* message;
  int numlines;
  int maxwidth;
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

typedef struct user_t
{
  int usernum;
  char* username;
} user_t;

pthread_mutex_t disp_mutex;

llist_t* MSGS;
llist_t* INFOS;

node_t* LAST_MSG_NODE;
node_t* LAST_INFO_NODE;

int showhelp;

char MY_MSG[1024];
int MY_MSG_INDEX;

char* uihostname;
int uiport;

int UIRUNNING;

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

void initui(int isdebug);
void print_msg(char* user, char message[]);
void print_info(char* user, char message[]);
void print_msg_with_senderids(char* user, char message[], char hostname[], int portnum);
uimessage_t* add_msg_with_senderids(char* user, char message[], llist_t* msglist, int append, char hostname[], int portnum);
void setborder(window_t* wnd);
