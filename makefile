CC=cc
BLOCKGAME=blockgame

# Generated with `gcc -MM -Iinclude src/*.c`
log.o: src/log.c include/blockgame/log.h include/blockgame/panic.h \
 include/blockgame/macro.h
main.o: src/main.c include/blockgame/log.h

build: main.o
	$(CC) -o$(BLOCKGAME) -Iinclude src/*.c 

.PHONY: build