# Créé par Olivier EBLE

RM 	= rm
RMFLAGS	= -f
SH	= @bash
ECHO	= @echo
CC	= gcc
CCFLAGS	= -Wall -pedantic -m32 -g -I ../libs/gMemory/
EDL	= gcc
LDFLAGS	= -m32 -g
LIBS = -lm -lpthread -lrt
SRC	= $(wildcard *.c) $(wildcard gCommunication/*.c) $(wildcard lib/*.c) $(wildcard sensors/*.c) $(wildcard ../libs/gMemory/gMemory.c)
OBJS	= $(SRC:.c=.o)
EXE     = gBox



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

