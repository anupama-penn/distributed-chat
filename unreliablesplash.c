#define _XOPEN_SOURCE_EXTENDED 1

#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <ncursesw/curses.h>
#include <locale.h>
#include <wchar.h>



int r,c,nrows,ncols;
WINDOW *wnd;

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

void splashcurses()
{
  init_pair(1, COLOR_BLUE, COLOR_BLACK);
  init_pair(2, COLOR_CYAN, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_WHITE, COLOR_BLACK);
  attron(COLOR_PAIR(1));
  attron(A_BOLD);
  //Unreliable
  mvaddwstr(1,0,L"  $$$$  $$$$                                  $$$$ $$$$            $$$$       $$$$");
  mvaddwstr(2,0,L"  $$$$  $$$$                                  $$$$ ****            $$$$       $$$$");
  mvaddwstr(3,0,L"  $$$$  $$$$ $$$o$$$o.  $$$o$$$o.   .o$$$$o.  $$$$ $$$$  .o$$$$$o. $$$$o$$o.  $$$$  .o$$$$o.");
  mvaddwstr(4,0,L"  $$$$  $$$$ $$$$  $$$o $$$$  $$$o .$$$  $$$o $$$$ $$$$  ***  $$$$ $$$$  $$$. $$$$ .$$$  $$$o");
  mvaddwstr(5,0,L"  $$$$  $$$$ $$$$  $$$$ $$$$  $$$$ $$$$$$$$$$ $$$$ $$$$  o$$$$$$$$ $$$$  $$$$ $$$$ $$$$$$$$$$");
  mvaddwstr(6,0,L"  $$$$  $$$$ $$$$  $$$$ $$$$       *$$$  oooo $$$$ $$$$ $$$$  $$$$ $$$$  $$$* $$$$ *$$$  oooo");
  mvaddwstr(7,0,L"  '*$$$$*$$$ $$$$  $$$$ $$$$        '*$$$$$*' $$$$ $$$$  *$$$$*$$$ $$$*$$$*'  $$$$  '*$$$$$*'");
  attroff(COLOR_PAIR(1));
  attron(COLOR_PAIR(2));
  //Chat
  mvaddwstr(9,0,L"   .o$$$$o.  $$$$");
  mvaddwstr(10,0,L"  o$$$  $$$o $$$$                  $$$$");
  mvaddwstr(11,0,L"  $$$$  $$$$ $$$$$$$o.   .o$$$$$o. $$$$$$");
  mvaddwstr(12,0,L"  $$$$       $$$$  $$$o  ***  $$$$ $$$$");
  mvaddwstr(13,0,L"  $$$$  $$$$ $$$$  $$$$  o$$$$$$$$ $$$$");
  mvaddwstr(14,0,L"  *$$$  $$$* $$$$  $$$$ $$$$  $$$$ $$$$");
  mvaddwstr(15,0,L"   '*$$$$*'  $$$$  $$$$  *$$$$*$$$ '*$$$$");
  attroff(COLOR_PAIR(2));

  //credits
  attron(COLOR_PAIR(4));
  mvaddwstr(17,0,L"  $$$$$$$$$$");
  mvaddwstr(18,0,L"  $$$$  $$$$");
  mvaddwstr(19,0,L"  $$$$$$$$$$");
  mvaddwstr(17,18,L" #1");
  mvaddwstr(18,18,L" #2");
  mvaddwstr(19,18,L" #3");
  attroff(COLOR_PAIR(4));
  attron(COLOR_PAIR(1));
  mvaddwstr(19,14,L"くコB彡");
  mvaddwstr(17,14,L"くコ8彡");
  attroff(COLOR_PAIR(1));
  attron(COLOR_PAIR(2));
  mvaddwstr(18,14,L"くコ:ミ");
  attroff(COLOR_PAIR(2));
  attron(COLOR_PAIR(3));
  mvaddwstr(17,22,L"Spencer Caplan");
  mvaddwstr(18,22,L"Jordan~ Kodner");
  mvaddwstr(19,22,L"Anupama Kumar~");
  attroff(COLOR_PAIR(3));

  //sotong tank
  mvaddwstr(9,43,L"o$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$o");
  mvaddwstr(10,43,L"$$                                              $$");
  mvaddwstr(11,43,L"$$       くコBミ  くコ:彡       How so Blur?    $$");
  mvaddwstr(12,43,L"$$  くコ8彡           くコ8ミ  くコ:彡          $$");
  mvaddwstr(13,43,L"$$               How so Blur?          くコ8ミ  $$");
  mvaddwstr(14,43,L"$$    くコB彡    くコBミ         くコ8彡        $$");
  mvaddwstr(15,43,L"$$            くコ8彡  How so Blur? くコ:ミ     $$");
  mvaddwstr(16,43,L"$$ くコ:彡           くコ8ミ      くコB彡       $$");
  mvaddwstr(17,43,L"$$       How so Blur?     くコBミ       くコ8彡 $$");
  mvaddwstr(18,43,L"$$                                              $$");
  mvaddwstr(19,43,L"*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*");


  attroff(A_BOLD);
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

void draw(char dc)
{
  move(r,c);
  delch();
  insch(dc);
  r++;
  if(r == nrows)
  {
    r = 0;
    c++;
    if (c== ncols)
      c = 0;
  }
}

int main(int argc, char** argv)
{
  int i = 0;

  char d;
  WINDOW *wnd;
  
  //  splash();
  wnd = initscr();
  setlocale(LC_ALL, "en_US.utf8");
  //  if(has_colors() == FALSE)
  //    {endwin();
  //      printf("Your terminal does not support color. Run in non-UI mode instead.\n");
  //      exit(1);
  //    }
  start_color();
  cbreak();
  noecho();
  getmaxyx(wnd,nrows,ncols);
  //  clear();
  refresh();
  splashcurses();
  wrefresh(wnd);
  r = 0;
  c = 0;
  while(1)
  {
    d = getch();
    if(d == 'q') break;
    draw(d);
  }


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
