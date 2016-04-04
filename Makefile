PARENTDIR=..
CFLAGS=-g -Wall -I$(PARENTDIR)
CC=gcc
LDFLAGS=
LIBS=


all: dchat.o llist.o queue.o
	$(CC) $(CFLAGS) -o dchat dchat.o llist.o queue.o

dchat.o : dchat.c dchat.h llist.c llist.h queue.c queue.h
	$(CC) $(CFLAGS) -c dchat.c dchat.h queue.h queue.c llist.c llist.h

clean :
	rm -f *.o *.gch dchat



#part1 : server-udp.o change-password-udp.o add-voter-udp.o vote-udp.o vote-count-udp.o vote-zero-udp.o list-candidates-udp.o view-result-udp.o llist.o udp.o
#	$(CC) $(CFLAGS) -o server-udp server-udp.o llist.o udp.o
#	$(CC) $(CFLAGS) -o change-password-udp change-password-udp.o udp.o
#	$(CC) $(CFLAGS) -o add-voter-udp add-voter-udp.o udp.o
#	$(CC) $(CFLAGS) -o vote-udp vote-udp.o udp.o
#	$(CC) $(CFLAGS) -o vote-count-udp vote-count-udp.o udp.o
#	$(CC) $(CFLAGS) -o vote-zero-udp vote-zero-udp.o udp.o
#	$(CC) $(CFLAGS) -o list-candidates-udp list-candidates-udp.o udp.o
#	$(CC) $(CFLAGS) -o view-result-udp view-result-udp.o udp.o
#
#view-result-udp.o : view-result-udp.c view-result-udp.h udp.c udp.h
#	$(CC) $(CFLAGS) -c view-result-udp.c view-result-udp.h udp.c udp.h
#
#list-candidates-udp.o : list-candidates-udp.c list-candidates-udp.h udp.c udp.h
#	$(CC) $(CFLAGS) -c list-candidates-udp.c list-candidates-udp.h udp.c udp.h
#
#vote-zero-udp.o : vote-zero-udp.c vote-zero-udp.h udp.c udp.h
#	$(CC) $(CFLAGS) -c vote-zero-udp.c vote-zero-udp.h udp.c udp.h
#
#vote-count-udp.o : vote-count-udp.c vote-count-udp.h udp.c udp.h
#	$(CC) $(CFLAGS) -c vote-count-udp.c vote-count-udp.h udp.c udp.h
#
#vote-udp.o : vote-udp.c vote-udp.h udp.c udp.h
#	$(CC) $(CFLAGS) -c vote-udp.c vote-udp.h udp.c udp.h
#
#add-voter-udp.o : add-voter-udp.c add-voter-udp.h udp.c udp.h
#	$(CC) $(CFLAGS) -c add-voter-udp.c add-voter-udp.h udp.c udp.h
#
##change-password-udp.o : change-password-udp.c change-password-udp.h $(PARENTDIR)/constants.h
##	$(CC) $(CFLAGS) -c change-password-udp.c change-password-udp.h $(PARENTDIR)/constants.h
#
##server-udp.o : server-udp.c server-udp.h $(PARENTDIR)/constants.h $(PARENTDIR)/llist.c $(PARENTDIR)/llist.h
##	$(CC) $(CFLAGS) -c server-udp.c server-udp.h $(PARENTDIR)/constants.h $(PARENTDIR)/llist.c $(PARENTDIR)/llist.h
#
#change-password-udp.o : change-password-udp.c change-password-udp.h udp.c udp.h
#	$(CC) $(CFLAGS) -c change-password-udp.c change-password-udp.h udp.c udp.h
#
#server-udp.o : server-udp.c server-udp.h udp.h udp.c $(PARENTDIR)/llist.c $(PARENTDIR)/llist.h
#	$(CC) $(CFLAGS) -c server-udp.c server-udp.h udp.h udp.c $(PARENTDIR)/llist.c $(PARENTDIR)/llist.h
#
#clean :
#	rm -f *.o *.gch server-udp change-password-udp add-voter-udp *-udp
#
