#define _XOPEN_SOURCE_EXTENDED 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <ncursesw/curses.h>
#include <locale.h>
#include <wchar.h>

typedef struct window_t
{
  WINDOW* window;
  int r;
  int c;
  int nrows;
  int ncols;
  int hasfocus;
} window_t;


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



void draw(char dc)
{
  wmove(focuswnd->window,r,c);
  wdelch(focuswnd->window);
  winsch(focuswnd->window,dc);
  c++;
  if(c == focuswnd->ncols)
  {
    c = 2;
    r++;
    if (r== focuswnd->nrows)
      r = 2;
  }
  wmove(focuswnd->window,r,c);
}

void setborder(window_t* wnd)
{
  int color = 10;
  if(wnd->hasfocus == 1)
  {
    color = 11;
  }
  wattron(wnd->window,COLOR_PAIR(color));
  wborder(wnd->window, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wattroff(wnd->window,COLOR_PAIR(color));
}

window_t* init_wnd(int h, int w, int startr, int startc)
{
  window_t* newwnd = (window_t*)malloc(sizeof(window_t));;
  WINDOW* newwindow;

  newwindow = newwin(h,w,startr,startc);
  newwnd->window = newwindow;
  newwnd->r = 0;
  newwnd->c = 0;
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
  setborder(focuswnd);
  newfocuswnd->hasfocus = 1;
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

  //  splash();
  mainwnd = initscr();
  setlocale(LC_ALL, "en_US.utf8");
  start_color();
  init_pair(10, COLOR_BLACK, COLOR_WHITE);
  init_pair(11, COLOR_BLACK, COLOR_CYAN);


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
  focusindex = 0;
  focuswnd = focusable_wnds[0];
  nextfocus();
  //  setfocus(inputwnd);
  

  splashcurses();

  r = 2;
  c = 2;
  while(1)
  {
    d = wgetch(focuswnd->window);
    if(d == '\t')
      nextfocus();
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
