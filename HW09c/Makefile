CFLAGS+=--std=c99 
CFLAGS+=-g
CFLAGS+=-O2 -Wall -Werror -pedantic

all: graph.o txt2bin bin2txt txt2txt graph_creator main

graph_creator: graph_creator.c
	$(CC) $(CFLAGS) $< -o $@

my_malloc: my_malloc.c my_malloc.h
	$(CC) $(CFLAGS) -c my_malloc.c -o my_malloc.o

graph.o: graph.c graph.h
	$(CC) $(CFLAGS) -c graph.c -o graph.o

main: main.c
	$(CC) $(CFLAGS) main.c -o main

txt2bin: txt2bin.c graph.o
	$(CC) $(CFLAGS) $< graph.o -o $@

bin2txt: bin2txt.c graph.o
	$(CC) $(CFLAGS) $< graph.o -o $@

txt2txt: txt2txt.c graph.o
	$(CC) $(CFLAGS) $< graph.o -o $@
	
clean:
	rm -f *.o
	rm -f txt2bin bin2txt graph_creator 

zip:
	zip hw09-b0b36-brute.zip graph.h graph.c

