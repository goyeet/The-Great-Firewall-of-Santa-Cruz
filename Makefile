CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra
LFLAGS = -lm

all: banhammer

banhammer: banhammer.o
	$(CC) -o banhammer banhammer.o speck.o ht.o bst.o node.o bf.o bv.o parser.o $(LFLAGS)

banhammer.o: banhammer.c speck.c ht.c bst.c node.c bf.c bv.c parser.c
	$(CC) $(CFLAGS) -c banhammer.c speck.c ht.c bst.c node.c bf.c bv.c parser.c

debug: CFLAGS += -g
debug: all

clean:
	rm -f banhammer banhammer.o *.o

format:
	clang-format -i -style=file *.[ch]

