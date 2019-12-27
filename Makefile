CC = gcc
CFLAGS = -W -Wall -Wextra -std=90

ifeq ($(OSTYPE),WINDOWS)
	EXECEXT =.exe
	COMP	=__MINGCC__
	PLATFORM	=mingw
else
	EXECEXT =
	COMP	=__GCC__
	PLATFORM	=linux
endif

EXECUTABLES= scrabble$(EXECEXT)

all : $(EXECUTABLES)

chainedHashTable.o : chainedHashTable.c abstractHashTable.h 
	gcc -c chainedHashTable.c

CheckBoard.o : CheckBoard.c abstractHashTable.h struct.h 
	gcc -c CheckBoard.c

struct.o : struct.c
	gcc -c struct.c

scrabble.o : scrabble.c CheckBoard.h abstractHashTable.h struct.h
	gcc -c scrabble.c

save.o : save.c abstractHashTable.h struct.h
	gcc -c save.c

scrabble$(EXECEXT) : chainedHashTable.o CheckBoard.o struct.o scrabble.o save.o
	gcc -o scrabble$(EXECEXT) scrabble.o CheckBoard.o struct.o chainedHashTable.o save.o 

clean : 
	-rm *.o
	-rm $(EXECUTABLES) 
