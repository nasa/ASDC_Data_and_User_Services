
#####################################sccsidkeywordsstart################
#     *** This file is under configuration control
#     *** sccs keywords--do not modify these values!!!
#     @(#)Makefile    93.5    00/01/10
#####################################sccsidkeywordsend##################





#SHELL = /usr/local/bin/tcsh

FFLAGS =  -O -u -C
CFLAGS =  -O 
CC = /usr/ucb/cc
CC = /usr/local/bin/gcc

.KEEP_STATE:

OBJS_C =     io.o  gbytes.o

LIBS=	-lifsql -lifgen -lifos -lifgls -lifasf -lifglx -lnsl -lsocket \
	/usr/local/informix/lib/esql/checkapi.o

FFLAGS = -c

erbelib.a:  $(OBJS_C)
	$(AR) $(ARFLAGS) $@ $?
	ranlib $@
	f77 -o mtsa_reads10.exe readpgm.f erbelib.a 

.SUFFIXES: .prt .f .c .o .ec

.f.o:
	$(COMPILE.f) $<

.c.o:
	$(COMPILE.c) $<

