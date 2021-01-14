#############################################
# CUSTOM Modules Makefile
#############################################
#CFLAGS=-g -Ae +DD64 +DO11.11
#LINKFLAGS=+DD64 +DO11.11 -lsec -lm
CFLAGS=-std=c11 -g -Og -Wall -m64 -D_XOPEN_SOURCE
LINKFLAGS=-lm
CC=gcc
INCLUDES=-I/usr/include
#LINK_LIB=/usr/lib/hpux64/libisam.a /usr/lib/hpux64/libm.a
LINK_LIB=

#############################################
# Object files
#############################################

CDR_OBJS=frm_cdr_feed.o frm_cdr_mapp.o strlogutl.o
BIN=/appl1/hpe/devsrc/preproc/cdr_feed/bin
#############################################
# Actions & Dependencies
#############################################

all: frm_cdr_feed.exe

frm_cdr_feed.exe	: $(CDR_OBJS)
	$(CC) $(LINKFLAGS) $(CDR_OBJS) $(LINK_LIB) -o $@
	cp -p frm_cdr_feed.exe ${BIN}

frm_cdr_feed.o		: frm_cdr_feed.c frm_cdr_feed.h frm_cdr_glob.h frm_cdr_stru.h
	$(CC) $(CFLAGS) $(INCLUDES) -c frm_cdr_feed.c -o $@

frm_cdr_mapp.o		: frm_cdr_mapp.c frm_cdr_mapp.h frm_cdr_glob.h frm_cdr_stru.h
	$(CC) $(CFLAGS) $(INCLUDES) -c  frm_cdr_mapp.c -o $@

strlogutl.o			: strlogutl.c strlogutl.h
	$(CC) $(CFLAGS) $(INCLUDES) -c  strlogutl.c -o $@

clean				:
	rm -f ${CDR_OBJS} ${BIN}/frm_cdr_feed.exe
