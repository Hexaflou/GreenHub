# Créé par Olivier EBLE

RM 	= rm
RMFLAGS	= -f
SH	= @bash
ECHO	= @echo
CC	= gcc
CCFLAGS	= -I ../libhw/include -Wall -pedantic -m32 -g
EDL	= gcc
LDFLAGS	= -m32 -g 
LIBS = ../libhw/lib/libhw.a -lm -lpthread -lrt 
SRC	= $(wildcard *.c) 
OBJS	= $(SRC:.c=.o)
EXE     = test



all: $(EXE)

%.o : %.c
	$(ECHO) compilation de $<
	$(CC) $(CCFLAGS) -o $@ -c $<

affiche :
	$(ECHO) $(SRC)

$(EXE): $(OBJS)
	$(ECHO) Edition des liens de $(EXE)
	$(EDL) $(LDFLAGS) -o $(EXE) $(OBJS) $(LIBS)

clean:
	$(ECHO) nettoyage des binaires translatables
	$(RM) $(RMFLAGS) $(OBJS) core

cleanall: clean
	$(ECHO) nettoyage de l executable
	$(RM) $(RMFLAGS) $(EXE)

