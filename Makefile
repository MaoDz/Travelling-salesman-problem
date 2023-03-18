# options de compilation
CC = gcc
CCFLAGS = -Wall -g
LIBSDIR =
LDFLAGS =-lm

# fichiers du projet
SRC = site.c haversine.c lectureFichiers.c  tableauSite.c algo.c affichage.c main.c
OBJ = $(SRC:.c=.o)
EXEC = site

# règle initiale
all : $(EXEC)

# dépendance des .h
site.o: site.h
heversine.o: haversine.h
lectureFichiers.o: lectureFichiers.h
tableauSite.o: tableauSite.h
algo.o: algo.h haversine.h affichage.h site.h
main.o: algo.h tableauSite.h affichage.h
affichage.o: affichage.h

# règles de compilation
%.o:%.c
	$(CC) $(CCFLAGS) -o $@ -c $<

#règle d'édition de liens
$(EXEC) :$(OBJ)
	$(CC) -g -o $@ $^ $(LIBSDIR) $(LDFLAGS)

#règles suplémentaires
clean :
	rm -f *.o $(EXEC)
