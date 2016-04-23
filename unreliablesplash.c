#include "unreliablesplash.h"

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
  printf("\E[1m\E[37m  $$$$$$$$$$  \E[34mくコB彡 \E[37m#3  \E[25m\E[33mAnupama くコ:ミ\E[25m  \E[37m*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*\E(B\E[m\n");
  printf("\n");
}

void *splashcurses(void *t)
{
  init_pair(1, COLOR_BLUE, COLOR_BLACK);
  init_pair(2, COLOR_CYAN, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_WHITE, COLOR_BLACK);
  init_pair(5, COLOR_RED, COLOR_BLACK);
  init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(7, COLOR_GREEN, COLOR_BLACK);

  pthread_mutex_lock(&disp_mutex);
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
  mvwaddwstr(splashwnd->window,18,22,L" #1");
  mvwaddwstr(splashwnd->window,19,22,L" #2");
  mvwaddwstr(splashwnd->window,20,22,L" #3");
  wattroff(splashwnd->window,COLOR_PAIR(4));
  wattron(splashwnd->window,COLOR_PAIR(3));
  mvwaddwstr(splashwnd->window,18,27,L"Spencer Caplan");
  mvwaddwstr(splashwnd->window,19,27,L"Jordan  Kodner");
  mvwaddwstr(splashwnd->window,20,27,L"Anupama くコ:ミ");
  wattroff(splashwnd->window,COLOR_PAIR(3));
  pthread_mutex_unlock(&disp_mutex);

  while(1){
    pthread_mutex_lock(&disp_mutex);
    mvwaddwstr(splashwnd->window,10,44,L"o$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$o");
    mvwaddwstr(splashwnd->window,11,44,L"$$                                              $$");
    mvwaddwstr(splashwnd->window,12,44,L"$$                                              $$");
    mvwaddwstr(splashwnd->window,13,44,L"$$                                              $$");
    mvwaddwstr(splashwnd->window,14,44,L"$$                                              $$");
    mvwaddwstr(splashwnd->window,15,44,L"$$                                              $$");
    mvwaddwstr(splashwnd->window,16,44,L"$$                                              $$");
    mvwaddwstr(splashwnd->window,17,44,L"$$                                              $$");
    mvwaddwstr(splashwnd->window,18,44,L"$$                                              $$");
    mvwaddwstr(splashwnd->window,19,44,L"$$                                              $$");
    mvwaddwstr(splashwnd->window,20,44,L"*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*");

  //sotong
    wattron(splashwnd->window,COLOR_PAIR(2));
    mvwaddwstr(splashwnd->window,12,64,L"くコ:ミ");
    mvwaddwstr(splashwnd->window,13,48,L"くコ8ミ                 くコ:ミ");
    mvwaddwstr(splashwnd->window,15,75,L"くコ8ミ");
    mvwaddwstr(splashwnd->window,16,56,L"くコ8ミ");
    mvwaddwstr(splashwnd->window,17,49,L"くコ:ミ                    くコBミ");
    mvwaddwstr(splashwnd->window,18,72,L"くコB彡");
    wattroff(splashwnd->window,COLOR_PAIR(2));
    wattron(splashwnd->window,COLOR_PAIR(1));
    mvwaddwstr(splashwnd->window,12,55,L"くコB彡");
    mvwaddwstr(splashwnd->window,13,64,L"くコ8彡");
    mvwaddwstr(splashwnd->window,14,79,L"くコ8彡");
    mvwaddwstr(splashwnd->window,15,49,L"くコBミ      くコB彡");
    mvwaddwstr(splashwnd->window,16,82,L"くコ:彡");
    mvwaddwstr(splashwnd->window,17,67,L"くコ8彡");
    mvwaddwstr(splashwnd->window,18,81,L"くコ8ミ");
    wattroff(splashwnd->window,COLOR_PAIR(1));
    wattron(splashwnd->window,COLOR_PAIR(1));
    mvwaddwstr(splashwnd->window,20,15,L"くコB彡");
    mvwaddwstr(splashwnd->window,18,15,L"くコ8彡");
    wattroff(splashwnd->window,COLOR_PAIR(1));
    wattron(splashwnd->window,COLOR_PAIR(2));
    mvwaddwstr(splashwnd->window,19,15,L"くコ:ミ");
    wattroff(splashwnd->window,COLOR_PAIR(2));

  //How so Blur?
    wattron(splashwnd->window,COLOR_PAIR(3));
    mvwaddwstr(splashwnd->window,12,75,L"UnreliableChat");
    mvwaddwstr(splashwnd->window,14,61,L"How so Blur?");
    mvwaddwstr(splashwnd->window,16,66,L"UnreliableChat");
    mvwaddwstr(splashwnd->window,18,53,L"How so Blur?");
    wattroff(splashwnd->window,COLOR_PAIR(3));
    
    wrefresh(splashwnd->window);
    pthread_mutex_unlock(&disp_mutex);
    sleep(1);
    pthread_mutex_lock(&disp_mutex);
    mvwaddwstr(splashwnd->window,10,44,L"o$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$o");
    mvwaddwstr(splashwnd->window,11,44,L"$$                                              $$");
    mvwaddwstr(splashwnd->window,12,44,L"$$                                              $$");
    mvwaddwstr(splashwnd->window,13,44,L"$$                                              $$");
    mvwaddwstr(splashwnd->window,14,44,L"$$                                              $$");
    mvwaddwstr(splashwnd->window,15,44,L"$$                                              $$");
    mvwaddwstr(splashwnd->window,16,44,L"$$                                              $$");
    mvwaddwstr(splashwnd->window,17,44,L"$$                                              $$");
    mvwaddwstr(splashwnd->window,18,44,L"$$                                              $$");
    mvwaddwstr(splashwnd->window,19,44,L"$$                                              $$");
    mvwaddwstr(splashwnd->window,20,44,L"*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*");
    
    //sotong
    wattron(splashwnd->window,COLOR_PAIR(2));
    mvwaddwstr(splashwnd->window,12,62,L"くコ:彡");
    mvwaddwstr(splashwnd->window,13,47,L"くコ8彡                     くコ:彡");
    mvwaddwstr(splashwnd->window,15,77,L"くコ8彡");
    mvwaddwstr(splashwnd->window,16,58,L"くコ8彡");
    mvwaddwstr(splashwnd->window,17,47,L"くコ:彡                        くコB彡");
    mvwaddwstr(splashwnd->window,18,70,L"くコBミ");
    wattroff(splashwnd->window,COLOR_PAIR(2));
    wattron(splashwnd->window,COLOR_PAIR(1));
    mvwaddwstr(splashwnd->window,12,53,L"くコBミ");
    mvwaddwstr(splashwnd->window,13,66,L"くコ8ミ");
    mvwaddwstr(splashwnd->window,14,83,L"くコ8ミ");
    mvwaddwstr(splashwnd->window,15,50,L"くコB彡    くコBミ");
    mvwaddwstr(splashwnd->window,16,80,L"くコ:ミ");
    mvwaddwstr(splashwnd->window,17,65,L"くコ8ミ");
    mvwaddwstr(splashwnd->window,18,83,L"くコ8彡");
    wattroff(splashwnd->window,COLOR_PAIR(1));
    wattron(splashwnd->window,COLOR_PAIR(1));
    mvwaddwstr(splashwnd->window,20,15,L"くコBミ");
    mvwaddwstr(splashwnd->window,18,15,L"くコ8ミ");
    wattroff(splashwnd->window,COLOR_PAIR(1));
    wattron(splashwnd->window,COLOR_PAIR(2));
    mvwaddwstr(splashwnd->window,19,15,L"くコ:彡");
    wattroff(splashwnd->window,COLOR_PAIR(2));

  //How so Blur?
    wattron(splashwnd->window,COLOR_PAIR(3));
    mvwaddwstr(splashwnd->window,12,76,L"How so Blur?");
    mvwaddwstr(splashwnd->window,14,60,L"UnreliableChat");
    mvwaddwstr(splashwnd->window,16,67,L"How so Blur?");
    mvwaddwstr(splashwnd->window,18,52,L"UnreliableChat");
    wattroff(splashwnd->window,COLOR_PAIR(3));

    wrefresh(splashwnd->window);
    pthread_mutex_unlock(&disp_mutex);
    sleep(1);
    //    break;
  }

  wattroff(splashwnd->window,A_BOLD);
  pthread_exit((void *)t);
  //  wrefresh(splashwnd->window);
}

int get_color(uimessage_t* uimsg)
{
  int color = 0;
  if(strcmp(uimsg->user->uid,uiuid) == 0)
    //  if(strcmp(uimsg->user->hostname,uihostname) == 0 && uimsg->user->portnum == uiport)
  {
    color = 3;
  }
  else
  {
    color = (uimsg->user->usernum % 6) + 1;
    if(color == 3)
      color = 7;
  }
  return color;
}

user_t* add_user(char* username, char uid[])
{
  user_t* newuser = (user_t*)malloc(sizeof(user_t));
  newuser->username = (char*)malloc(sizeof(char)*100);
  newuser->uid = (char*)malloc(sizeof(char)*30);
  newuser->usernum = USERNUM;
  strcpy(newuser->username,username);
  strcpy(newuser->uid,uid);
  //  newuser->portnum = port;
  add_elem(USERS,newuser);
  USERNUM++;
  return newuser;
}

user_t* find_user_by_uid(char uid[])
{
  node_t* curr = USERS->head;
  while(curr != NULL)
  {
    user_t* elem = (user_t*)curr->elem;
    if(strcmp(elem->uid,uid) == 0)
      return elem;
    curr = curr->next;
  }
  return NULL;
}

uimessage_t* add_msg_with_senderids(char* user, char message[], llist_t* msglist, int append, char uid[])
{
  int maxline = msgwnd->ncols/2-2;
  int numlines = strlen(message)/maxline;
  if(strlen(message)%maxline > 0)
    numlines++;
  char* msglines = (char*)malloc(sizeof(char)*numlines*maxline+numlines);
  int lo = 0;
  int maxwidth = 0;
  if(strlen(message) > maxline)
  {
    strncpy(msglines,&message[lo],maxline);
    lo += maxline;
    strcat(msglines,"\n");
    maxwidth = maxline;
    while(lo+maxline < strlen(message))
    {
      strncat(msglines,&message[lo],maxline);
      lo += maxline;
      strcat(msglines,"\n");
    }
    strncat(msglines,&message[lo],strlen(&message[lo]));
    //    strncat(msglines,&message[lo],strlen(&message[lo]));
    strcat(msglines,"\n");
  }
  else
  {
    strcpy(msglines,&message[lo]);
    strcat(msglines,"\n");
    maxwidth = strlen(message);
    if(strlen(user) > maxwidth)
      maxwidth = strlen(user);
  }

  //get associated user
  user_t* msguser = find_user_by_uid(uid);
  if(msguser == NULL)
  {
    msguser = add_user(user,uid);
    printf("Added new user\n");
  }
  else if(strcmp(msguser->username,user) != 0)
    strcpy(msguser->username,user);

  if(msglist->tail != NULL && ((uimessage_t*)msglist->tail->elem)->user == msguser && append)
  {
    char* prevmsg = ((uimessage_t*)msglist->tail->elem)->message;
    char* combinedmsg = (char*)malloc(sizeof(char)*(strlen(msglines)+strlen(prevmsg)+1));
    strcpy(combinedmsg,prevmsg);
    strcat(combinedmsg,msglines);
    free(prevmsg);
    prevmsg = NULL;
    ((uimessage_t*)msglist->tail->elem)->message = combinedmsg;
    ((uimessage_t*)msglist->tail->elem)->numlines += numlines;
    if(maxwidth > ((uimessage_t*)msglist->tail->elem)->maxwidth)
      ((uimessage_t*)msglist->tail->elem)->maxwidth = maxwidth;
    return NULL;
  }
  else
  {
    uimessage_t* newmessage = (uimessage_t*)malloc(sizeof(uimessage_t));
    newmessage->user = msguser;
    newmessage->message = msglines;
    newmessage->numlines = numlines;
    newmessage->maxwidth = maxwidth;
    return newmessage;
  }

}

uimessage_t* add_msg(char* user, char message[], llist_t* msglist, int append)
{
  return add_msg_with_senderids(user, message, msglist, append, "");
}

void print_msgs()
{
  int linenum = msgwnd->nrows-4;

  node_t* curr = LAST_MSG_NODE;
  pthread_mutex_lock(&disp_mutex);
  wclear(msgwnd->window);
  pthread_mutex_unlock(&disp_mutex);
  setborder(msgwnd);
  while(curr != NULL)
  {
    uimessage_t* uimsg = (uimessage_t*)(curr->elem);
    int start = 10;
    int namestart = start-6;
    int color = 1;
    //    int namestart = 4;
    if(strcmp(uimsg->user->uid,uiuid) == 0)
    {
      start = msgwnd->ncols-4-uimsg->maxwidth;
      namestart = start-6;
    }
    color = get_color(uimsg);

    pthread_mutex_lock(&disp_mutex);
    int i = 0;
    char currchar = ' ';
    linenum-=uimsg->numlines;
    msgwnd->r = linenum;
    msgwnd->c = start;
    //    linenum += 1;
    currchar = uimsg->message[i];
    while(currchar != '\0')
    {     
      if(currchar == '\n')
      {
	linenum++;
	msgwnd->r = linenum;
	msgwnd->c = start;
	i++;
	currchar = uimsg->message[i];
	continue;
      }
      if(linenum > 0)
	mvwaddch(msgwnd->window,msgwnd->r,msgwnd->c,currchar);
      msgwnd->c++;
      i++;
      currchar = uimsg->message[i];
    }
    linenum-=uimsg->numlines;
    /*for(i = uimsg->numlines-1; i >= 0; i--)
    {
      //    mvwaddstr(msgwnd->window,msgwnd->nrows-2-i, 38, "hello");
      mvwaddstr(msgwnd->window,linenum, start, uimsg->message);
      linenum-=uimsg->numlines;
      break;
      //   linenum--;

      if(linenum == 0)
	break;
	}*/
    pthread_mutex_unlock(&disp_mutex);
    linenum--;
    pthread_mutex_lock(&disp_mutex);
    wattron(msgwnd->window,COLOR_PAIR(color));    
    int j = 0;
    for(j = namestart-2; j < uimsg->maxwidth+start+2; j++)
    {
      if(linenum-2 > 0)
      	mvwaddch(msgwnd->window,linenum-1,j,'*');
      if(linenum+uimsg->numlines+1 > 0)
	mvwaddch(msgwnd->window,linenum+uimsg->numlines+2,j,'*');
    }
    for(j = linenum-1; j < linenum+uimsg->numlines+2; j++)
    {
      if(j > 0)
      {
      	mvwaddch(msgwnd->window,j,namestart-2,'*');
      	mvwaddch(msgwnd->window,j,uimsg->maxwidth+start+1,'*');
      }
    }
    //    if(linenum < 1)
    //      break;
    wattron(msgwnd->window,A_BOLD);
    if(linenum > 0)
      mvwaddstr(msgwnd->window,linenum, namestart, uimsg->user->username);
    wattroff(msgwnd->window,A_BOLD);
    if(linenum <= 0)
    {
      wattroff(msgwnd->window,COLOR_PAIR(color));    
      pthread_mutex_unlock(&disp_mutex);
      break;
    }
    linenum-=3;
    /*    if(curr->prev != NULL)
    {
      if(strcmp(((uimessage_t*)curr->prev->elem)->username, uimsg->username))
	{
	mvwaddstr(msgwnd->window,linenum, namestart, uimsg->username);
	linenum-=3;
	}
    }
    else
    {
      mvwaddstr(msgwnd->window,linenum, namestart, uimsg->username);
      linenum-=3;
      }*/
    wattroff(msgwnd->window,COLOR_PAIR(color));    
    pthread_mutex_unlock(&disp_mutex);
    curr=curr->prev;
  }
  pthread_mutex_lock(&disp_mutex);
  if(MSGS->tail != LAST_MSG_NODE)
  {
    mvwaddstr(msgwnd->window,msgwnd->nrows-2,1," V V V V ");
    wattron(msgwnd->window,COLOR_PAIR(2));    
    wattron(msgwnd->window,A_BOLD);
    mvwaddstr(msgwnd->window,msgwnd->nrows-2,9," MORE MESSAGES BELOW ");
    wattroff(msgwnd->window,A_BOLD);
    wattroff(msgwnd->window,COLOR_PAIR(2));    
    int i = 0;
    for(i = 30; i < msgwnd->ncols-1; i+=2)
      mvwaddch(msgwnd->window,msgwnd->nrows-2,i,'V');
  }
  wrefresh(msgwnd->window);
  wrefresh(inputwnd->window);
  pthread_mutex_unlock(&disp_mutex);
}

void print_infos()
{
  if(showhelp)
  {
    pthread_mutex_lock(&disp_mutex);
    wclear(infownd->window);
    wattron(infownd->window,A_BOLD);
    wattron(infownd->window,COLOR_PAIR(3));    
    mvwaddwstr(infownd->window,2,infownd->ncols/2-16,L"くコ:彡                 くコ8ミ");
    wattroff(infownd->window,COLOR_PAIR(3));    
    wattron(infownd->window,COLOR_PAIR(1));    
    mvwaddstr(infownd->window,2,infownd->ncols/2-8,"Unreliable");
    wattroff(infownd->window,COLOR_PAIR(1));    
    wattron(infownd->window,COLOR_PAIR(2));    
    mvwaddstr(infownd->window,2,infownd->ncols/2+3,"Chat");
    wattroff(infownd->window,COLOR_PAIR(2));    
    wattron(infownd->window,COLOR_PAIR(4));    
    mvwaddstr(infownd->window,3,infownd->ncols/2-5,"HELP MENU");
    wattroff(infownd->window,COLOR_PAIR(4));    
    wattroff(infownd->window,A_BOLD);

    wattron(infownd->window,A_BOLD);
    wattron(infownd->window,COLOR_PAIR(4));    
    mvwaddstr(infownd->window,6,infownd->ncols-43,"<TAB>  cycle through windows");
    mvwaddstr(infownd->window,9,8,"<ENTER>");
    //    mvwaddstr(infownd->window,8,8,"<ARROWS>");
    mvwaddstr(infownd->window,10,8,"<BKSPACE>");
    mvwaddstr(infownd->window,12,8,"<UP/DOWN>");
    mvwaddstr(infownd->window,13,8,"<LEFT>");
    mvwaddstr(infownd->window,14,8,"<RIGHT>");
    mvwaddstr(infownd->window,16,8,"<H>");
    mvwaddstr(infownd->window,17,8,"<UP/DOWN>");
    mvwaddstr(infownd->window,18,8,"<LEFT>");
    mvwaddstr(infownd->window,19,8,"<RIGHT>");
    mvwaddstr(infownd->window,8,infownd->ncols-43,"###############");
    mvwaddstr(infownd->window,9,infownd->ncols-43,"##           ##");
    mvwaddstr(infownd->window,10,infownd->ncols-43,"##           ##");
    mvwaddstr(infownd->window,11,infownd->ncols-43,"###############");
    wattroff(infownd->window,COLOR_PAIR(4));   
    wattron(infownd->window,COLOR_PAIR(1));    
    mvwaddstr(infownd->window,15,3,"INFO Window");
    mvwaddstr(infownd->window,9,infownd->ncols-41,"UNRELIABLE");
    mvwaddstr(infownd->window,12,infownd->ncols-43,"###############");
    mvwaddstr(infownd->window,13,infownd->ncols-43,"##           ##");
    mvwaddstr(infownd->window,14,infownd->ncols-43,"##           ##");
    mvwaddstr(infownd->window,15,infownd->ncols-43,"##           ##");
    mvwaddstr(infownd->window,16,infownd->ncols-43,"###############");
    wattroff(infownd->window,COLOR_PAIR(1));    
    wattron(infownd->window,COLOR_PAIR(2));    
    mvwaddstr(infownd->window,11,3,"MESSAGE Window");
    mvwaddstr(infownd->window,10,infownd->ncols-41,"CHAT");
    mvwaddstr(infownd->window,8,infownd->ncols-28,"#####################");
    mvwaddstr(infownd->window,9,infownd->ncols-28,"##                 ##");
    mvwaddstr(infownd->window,10,infownd->ncols-28,"##                 ##");
    mvwaddstr(infownd->window,11,infownd->ncols-28,"##                 ##");
    mvwaddstr(infownd->window,12,infownd->ncols-28,"##                 ##");
    mvwaddstr(infownd->window,13,infownd->ncols-28,"##                 ##");
    mvwaddstr(infownd->window,14,infownd->ncols-28,"##                 ##");
    mvwaddstr(infownd->window,15,infownd->ncols-28,"##                 ##");
    mvwaddstr(infownd->window,16,infownd->ncols-28,"##                 ##");
    mvwaddstr(infownd->window,17,infownd->ncols-28,"##                 ##");
    mvwaddstr(infownd->window,18,infownd->ncols-28,"##                 ##");
    mvwaddstr(infownd->window,19,infownd->ncols-28,"#####################");
    wattroff(infownd->window,COLOR_PAIR(2));    
    wattron(infownd->window,COLOR_PAIR(3));    
    mvwaddstr(infownd->window,8,3,"INPUT Window");
    mvwaddstr(infownd->window,17,infownd->ncols-43,"###############");
    mvwaddstr(infownd->window,18,infownd->ncols-43,"##           ##");
    mvwaddstr(infownd->window,19,infownd->ncols-43,"###############");
    wattroff(infownd->window,COLOR_PAIR(3));    
    wattroff(infownd->window,A_BOLD);    

    wattron(infownd->window,COLOR_PAIR(4));   
    mvwaddstr(infownd->window,9,20,"send message");
    mvwaddstr(infownd->window,10,20,"backspace");
    mvwaddstr(infownd->window,12,20,"review messages");
    mvwaddstr(infownd->window,13,20,"skip to oldest");
    mvwaddstr(infownd->window,14,20,"skip to newest");
    mvwaddstr(infownd->window,16,20,"toggle mode");
    mvwaddstr(infownd->window,17,20,"review updates");
    mvwaddstr(infownd->window,18,20,"skip to oldest");
    mvwaddstr(infownd->window,19,20,"skip to newest");
    wattron(infownd->window,A_BOLD);    
    mvwaddstr(infownd->window,18,infownd->ncols-41,"INPUT");
    mvwaddstr(infownd->window,9,infownd->ncols-26,"MESSAGE");
    mvwaddstr(infownd->window,13,infownd->ncols-41,"INFO");
    wattroff(infownd->window,A_BOLD);    
    wattroff(infownd->window,COLOR_PAIR(4));   

    wattron(infownd->window,COLOR_PAIR(1));    
    wattron(infownd->window,A_BOLD);
    mvwaddstr(infownd->window,infownd->nrows-3,infownd->ncols-25,"<H> TO TOGGLE UPDATES");
    wattroff(infownd->window,COLOR_PAIR(1));    
    wattroff(infownd->window,A_BOLD);

    wrefresh(infownd->window);
    pthread_mutex_unlock(&disp_mutex);
    setborder(infownd);

    return;
  }

  int linenum = infownd->nrows-5;

  node_t* curr = LAST_INFO_NODE;
  pthread_mutex_lock(&disp_mutex);
  wclear(infownd->window);
  pthread_mutex_unlock(&disp_mutex);
  setborder(infownd);
  if(focuswnd == inputwnd)
  setborder(inputwnd);
  while(curr != NULL)
  {
    uimessage_t* uimsg = (uimessage_t*)(curr->elem);
    int namestart = 2;
    int start = namestart+1+strlen(uimsg->user->username);
    int color = get_color(uimsg);

    pthread_mutex_lock(&disp_mutex);
    int i = 0;
    char currchar = ' ';
    linenum-=uimsg->numlines-1;
    infownd->r = linenum;
    infownd->c = start;
    currchar = uimsg->message[i];
    while(currchar != '\0')
    {     
      if(currchar == '\n')
      {
	linenum++;
	infownd->r = linenum;
	infownd->c = start;
	i++;
	currchar = uimsg->message[i];
	continue;
      }
      if(linenum > 0)
	mvwaddch(infownd->window,infownd->r,infownd->c,currchar);
      infownd->c++;
      i++;
      currchar = uimsg->message[i];
    }
    linenum-=uimsg->numlines;
    //    pthread_mutex_unlock(&disp_mutex);
    //    linenum--;
    //    pthread_mutex_lock(&disp_mutex);
    wattron(infownd->window,COLOR_PAIR(color));    
    wattron(infownd->window,A_BOLD);
      mvwaddstr(infownd->window,linenum, namestart, uimsg->user->username);
    wattroff(infownd->window,A_BOLD);
    if(linenum <= 0)
    {
      wattroff(infownd->window,COLOR_PAIR(color));    
      pthread_mutex_unlock(&disp_mutex);
      break;
    }
    linenum--;
    wattroff(infownd->window,COLOR_PAIR(color));    
    pthread_mutex_unlock(&disp_mutex);
    curr=curr->prev;
  }
  pthread_mutex_lock(&disp_mutex);
  wattron(infownd->window,COLOR_PAIR(1));    
  wattron(infownd->window,A_BOLD);
  mvwaddstr(infownd->window,infownd->nrows-3,infownd->ncols-22,"<H> TO TOGGLE HELP");
  wattroff(infownd->window,COLOR_PAIR(1));    
  wattron(infownd->window,A_BLINK);
  if(unreadmessages)
  {
    wattron(infownd->window,COLOR_PAIR(3));    
    mvwaddstr(infownd->window,infownd->nrows-3,3,"UNREAD MESSAGES");
    wattron(infownd->window,COLOR_PAIR(3));    
  }
  if(unreadinfos)
  {
    wattron(infownd->window,COLOR_PAIR(3));    
    mvwaddstr(infownd->window,infownd->nrows-3,28,"UNREAD UPDATES");
    wattron(infownd->window,COLOR_PAIR(3));    
  }
  wattroff(infownd->window,A_BLINK);
  wattroff(infownd->window,A_BOLD);

  if(INFOS->tail != LAST_INFO_NODE)
  {
    mvwaddstr(infownd->window,infownd->nrows-2,1," V V V V ");
    wattron(infownd->window,COLOR_PAIR(1));    
    wattron(infownd->window,A_BOLD);
    mvwaddstr(infownd->window,infownd->nrows-2,9," MORE UPDATES BELOW ");
    wattroff(infownd->window,A_BOLD);
    wattroff(infownd->window,COLOR_PAIR(1));    
    int i = 0;
    for(i = 29; i < infownd->ncols-1; i+=2)
      mvwaddch(infownd->window,infownd->nrows-2,i,'V');
  }
  wrefresh(infownd->window);
  wrefresh(inputwnd->window);
  pthread_mutex_unlock(&disp_mutex);
}

void print_msg(char* user, char message[])
{
  print_msg_with_senderids(user,message,"");
}

void print_msg_with_senderids(char* user, char message[], char uid[])
{
  if(!UIRUNNING)
  {
    printf("\E[1m\E[33m%s\E(B\E[m:\t%s\n",user,message);
    return;
  }
  pthread_mutex_lock(&initui_mutex);
  pthread_mutex_unlock(&initui_mutex);
  uimessage_t* newmessage = add_msg_with_senderids(user,message,MSGS,1,uid);
  printf("Added new msg\n");
  if(newmessage)
  {
    int end = LAST_MSG_NODE == MSGS->tail;
    node_t* newtail = add_elem(MSGS, (void*)newmessage);
    if(end)
      LAST_MSG_NODE = newtail;
  }
  if(LAST_MSG_NODE != MSGS->tail)
  {
    unreadmessages = 1;
    print_infos();
  }
  print_msgs();
}

void print_info(char* user, char message[])
{
  print_info_with_senderids(user, message, "");
}

void print_info_with_senderids(char* user, char message[], char uid[])
{
  if(!UIRUNNING)
  {
    printf("\E[1m\E[36m%s\E(B\E[m:\t%s\n",user,message);
    return;
  }
  pthread_mutex_lock(&initui_mutex);
  pthread_mutex_unlock(&initui_mutex);
  uimessage_t* newmessage = add_msg_with_senderids(user,message,INFOS,0,uid);
  if(newmessage)
  {
    int end = LAST_INFO_NODE == INFOS->tail;
    node_t* newtail = add_elem(INFOS, (void*)newmessage);
    if(end)
      LAST_INFO_NODE = newtail;
  }
  if(LAST_INFO_NODE != INFOS->tail)
  {
    unreadinfos = 1;
    print_infos();
  }
  print_infos();
}

void *spoof_updates(void *t)
{
  while(1)
  {
    print_info("Alice","has joined the chat");
    sleep(2);
    print_info("Alice","has left the chat");
    sleep(3);
    print_info("Bob","is Bob.");
    sleep(4);
  }
  pthread_exit((void *)t);
}

void *spoof_chats(void *t)
{
  while(1)
  {
    print_msg("Alice", "Hi, how are you doing? I'm fine personally, but I'm just a function parameter, so I don't really know what I'm talking about. You know what I mean?");
    sleep(1);
    print_msg("Alice","Have you met Bob?");
    sleep(5);
    print_msg("Bob","Am Bob.");
    sleep(1);
    print_msg("Bob","Boon? Me ah?");
    sleep(2);
  }
  pthread_exit((void *)t);
}


int draw(char dc)
{
  wmove(focuswnd->window,focuswnd->r,focuswnd->c);
  if(dc == '\n')
  {
    //    wprintw(msgwnd->window,"\n\n   ");
    //    wprintw(msgwnd->window,MY_MSG);
    //    wrefresh(msgwnd->window);
    return 1;
  }
  if(dc == 127)//backspace
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
    waddch(focuswnd->window,' ');
    wmove(focuswnd->window, focuswnd->r, focuswnd->c);

    if(MY_MSG_INDEX > 0)
      MY_MSG_INDEX--;
    MY_MSG[MY_MSG_INDEX] = '\0';

    return 0;
  }

  if(focuswnd->c == focuswnd->ncols-2 && focuswnd->r == focuswnd->nrows-2)
    return 0;

  pthread_mutex_lock(&disp_mutex);
  wdelch(focuswnd->window);
  winsch(focuswnd->window,dc);
  pthread_mutex_unlock(&disp_mutex);

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
  return 0;
}



void setborder(window_t* wnd)
{
  int color = 10;
  pthread_mutex_lock(&disp_mutex);
  if(wnd->hasfocus == 1)
  {
    if(wnd == msgwnd)
      color = 11;
    else if(wnd == inputwnd)
      color = 12;
    else
      color = 13;
    wattron(wnd->window,A_BOLD);
  }
  //  if(wnd)
  //  draw(wnd->hasfocus+48);

  //  wborder(wnd->window, '│', ACS_VLINE, '-', '-', 'v', '>', '<', '^');
  wattron(wnd->window,COLOR_PAIR(color));
  //  wborder(wnd->window, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wborder(wnd->window, '#', '#', '#', '#', '#', '#', '#', '#');
  //  box(wnd->window, 0, 0);
  wattroff(wnd->window,COLOR_PAIR(color));
  wrefresh(wnd->window);
  wattroff(wnd->window,A_BOLD);
  pthread_mutex_unlock(&disp_mutex);
}

void refresh_wnd(window_t* wnd)
{
  pthread_mutex_lock(&disp_mutex);
  wclear(wnd->window);
  pthread_mutex_unlock(&disp_mutex);
  wnd->c = 2;
  wnd->r = 2;
  setborder(wnd);
  pthread_mutex_lock(&disp_mutex);
  wmove(focuswnd->window,focuswnd->r,focuswnd->c);
  wrefresh(wnd->window);
  pthread_mutex_unlock(&disp_mutex);
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
  pthread_mutex_lock(&disp_mutex);
  wrefresh(newwnd->window);
  pthread_mutex_unlock(&disp_mutex);
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
  int d;

  if(isdebug)
    return;


  USERNUM = 0;

  //  MY_MSG = (char*)malloc(sizeof(char));
  MY_MSG_INDEX = 0;
  USERS = (llist_t*)malloc(sizeof(llist_t));
  MSGS = (llist_t*)malloc(sizeof(llist_t));
  INFOS = (llist_t*)malloc(sizeof(llist_t));
  init_list(USERS);
  init_list(MSGS);
  init_list(INFOS);
  LAST_MSG_NODE = MSGS->tail;
  LAST_INFO_NODE = INFOS->tail;

  //  splash();
  mainwnd = initscr();
  //  keypad(mainwnd,TRUE);
  //  keypad(stdscr,TRUE);
  setlocale(LC_ALL, "en_US.utf8");
  start_color();

  init_pair(11, COLOR_CYAN, COLOR_BLACK);
  init_pair(12, COLOR_YELLOW, COLOR_BLACK);
  init_pair(13, COLOR_BLUE, COLOR_BLACK);
  init_pair(10, COLOR_WHITE, COLOR_BLACK);

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
  infownd = init_wnd(nrows-32, 97, 22, 1);
  inputwnd = init_wnd(11, 97, nrows-11, 1);
  msgwnd = init_wnd(nrows, ncols-99, 0, 98);
  keypad(msgwnd->window,TRUE);
  keypad(infownd->window,TRUE);
  focusable_wnds[0] = infownd;
  focusable_wnds[1] = inputwnd;
  focusable_wnds[2] = msgwnd;
  focusindex = 1;
  focuswnd = focusable_wnds[1];
  setfocus(inputwnd);

  //  splashcurses();

  pthread_t threads[3];
  pthread_attr_t attr;
  //  void *exitstatus;

  //make sure threads are joinable
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  //  pthread_create(&threads[SYSCALL_THREADNUM], &attr, spoof_chats, (void *)SYSCALL_THREADNUM);
  pthread_create(&threads[1], &attr, splashcurses, (void *)1);
  //  pthread_create(&threads[2], &attr, spoof_updates, (void *)2);

  //  pthread_join(threads[SYSCALL_THREADNUM], &exitstatus);

  unreadmessages = 0;
  unreadinfos = 0;
  showhelp = 0;

  pthread_mutex_unlock(&initui_mutex);
  int counter = 0;
  while(1)
  {
    d = wgetch(focuswnd->window);
    if(d == 4) //ctrl d
    {
      del_wnd(splashwnd);
      del_wnd(infownd);
      del_wnd(inputwnd);
      del_wnd(msgwnd);
      endwin();
      exit(1);
    }
    else if(d == '\t')
    {
      nextfocus();
    }
    else
    {
      if(focuswnd == inputwnd)
      {
	int done = draw((char)d);
	if(done)
	{
	  send_chat_message(counter,MY_MSG);
	  //	  print_msg("Me", MY_MSG);
	  refresh_wnd(focuswnd);
	  memset(MY_MSG,0, 1024);
	  MY_MSG_INDEX = 0;
	}
      }
      else if(focuswnd == msgwnd)
      {
	if(d == KEY_UP)
	{
	  if(LAST_MSG_NODE != MSGS->head)
	  {
	    LAST_MSG_NODE = LAST_MSG_NODE->prev;
	    print_msgs();
	  }
	}
	else if(d == KEY_DOWN)
	{
	  if(LAST_MSG_NODE != MSGS->tail)
	  {
	    LAST_MSG_NODE = LAST_MSG_NODE->next;
	    print_msgs();
	  }
	}
	else if(d == KEY_LEFT)
	{
	  LAST_MSG_NODE = MSGS->head;
	  print_msgs();
	}
	else if(d == KEY_RIGHT)
	{
	  LAST_MSG_NODE = MSGS->tail;
	  print_msgs();
	}
	if(LAST_MSG_NODE == MSGS->tail && unreadmessages == 1)
	{
	  unreadmessages = 0;
	  print_infos();
	}

      }
      else if(focuswnd == infownd)
      {
	if(d == KEY_UP)
	{
	  if(LAST_INFO_NODE != INFOS->head)
	  {
	    LAST_INFO_NODE = LAST_INFO_NODE->prev;
	    print_infos();
	  }
	}
	else if(d == KEY_DOWN)
	{
	  if(LAST_INFO_NODE != INFOS->tail)
	  {
	    LAST_INFO_NODE = LAST_INFO_NODE->next;
	    print_infos();
	  }
	}
	else if(d == KEY_LEFT)
	{
	  LAST_INFO_NODE = INFOS->head;
	  print_infos();
	}
	else if(d == KEY_RIGHT)
	{
	  LAST_INFO_NODE = INFOS->tail;
	  print_infos();
	}
	else if (d == 'h')
	{
	  if(showhelp)
	  {
	    showhelp = 0;
	    if(LAST_INFO_NODE == INFOS->tail && unreadmessages == 1)
	      unreadinfos = 0;
	  }
	  else
	    showhelp = 1;
	  print_infos();
	}
	if(LAST_INFO_NODE == INFOS->tail && unreadinfos == 1)
	{
	  unreadinfos = 0;
	  print_infos();
	}

      }
    }
  }
  
}

/*int main(int argc, char** argv)
{
  int i = 0;

  char d;

  UIRUNNING = 1;
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
*/
