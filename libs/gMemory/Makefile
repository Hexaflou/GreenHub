# Créé par Olivier EBLE

RM 	= rm
RMFLAGS	= -f
SH	= @bash
ECHO	= @echo
CC	= gcc
CCFLAGS	= -Wall -pedantic -m32 -g
EDL	= gcc
LDFLAGS	= -m32 -g 
LIBS = -lm -lrt 
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

