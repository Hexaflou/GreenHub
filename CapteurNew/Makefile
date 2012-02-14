# Créé par Olivier EBLE

RM 	= rm
RMFLAGS	= -f
SH	= @bash
ECHO	= @echo
CC	= gcc
CCFLAGS	= -Wall -pedantic -m32 -g -pipe -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -fasynchronous-unwind-tables -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -fno-strict-aliasing -fwrapv
EDL	= gcc
LDFLAGS	= -m32 -g -pipe -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -fasynchronous-unwind-tables -D_GNU_SOURCE -D_LARGEFILE_SOURCE -fno-strict-aliasing -fwrapv
LIBS = -rdynamic -lz -lcrypt -lnsl -lm -L/usr/lib -lssl -lcrypto -lpthread
SRC	= $(wildcard *.c)
OBJS	= $(SRC:.c=.o)
EXE     = Capteur



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
	
