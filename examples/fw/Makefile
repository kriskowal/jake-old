
P = fw
C = fw.c words.c string.c trie.c hash.c heap.c word.c
O = fw.o words.o string.o trie.o hash.o heap.o word.o
H = hash.h heap.h string.h trie.h word.h words.h
CC = gcc
CFLAGS = -Wall -O3 \
         -I/Developer/SDKs/MacOSX10.5.sdk/usr/include \
         -L/Developer/SDKs/MacOSX10.5.sdk/usr/lib

all: $(P)

$(P): $(O)
	$(CC) $(CFLAGS)  $(O) -o $(P)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

depend:
	   @makedepend -Y -- $(CFLAGS) -- $(C)

tags: $(C) $(H)
	etags $(C) $(H)

clean:
	rm fw $(O)

# DO NOT DELETE

fw.o: words.h trie.h word.h string.h hash.h heap.h
words.o: words.h trie.h word.h string.h hash.h heap.h
string.o: string.h
trie.o: trie.h word.h string.h
hash.o: hash.h trie.h word.h string.h
heap.o: heap.h word.h string.h
word.o: word.h string.h

