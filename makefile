# build.py variables:
#  - GCC_MM_OUTPUT

CC=cc
CFLAGS=-Wall -Iinclude
LDFLAGS=-lSDL2 -lepoxy -lm
BLOCKGAME=blockgame

CLANG_FORMAT=clang-format

# Generated with `gcc -MM -Iinclude src/*.c`
camera.o: src/camera.c include/blockgame/camera.h \
 include/blockgame/mathtypes.h include/blockgame/log.h \
 include/blockgame/math.h
engine.o: src/engine.c include/blockgame/engine.h include/blockgame/log.h \
 include/blockgame/math.h include/blockgame/mathtypes.h \
 include/blockgame/camera.h include/blockgame/file.h \
 include/blockgame/shader.h include/blockgame/vertex.h
file.o: src/file.c include/blockgame/file.h include/blockgame/log.h \
 include/blockgame/utility.h
log.o: src/log.c include/blockgame/log.h include/blockgame/panic.h \
 include/blockgame/macro.h
main.o: src/main.c include/blockgame/log.h include/blockgame/engine.h \
 include/blockgame/file.h
material.o: src/material.c include/blockgame/material.h \
 include/blockgame/model.h include/blockgame/vector.h src/str.h \
 include/blockgame/file.h include/blockgame/utility.h \
 include/blockgame/panic.h include/blockgame/macro.h \
 include/blockgame/log.h
model.o: src/model.c include/blockgame/model.h include/blockgame/vector.h \
 include/blockgame/file.h include/blockgame/utility.h \
 include/blockgame/panic.h include/blockgame/macro.h \
 include/blockgame/log.h src/str.h
shader.o: src/shader.c include/blockgame/shader.h \
 include/blockgame/mathtypes.h include/blockgame/log.h \
 include/blockgame/utility.h
str.o: src/str.c src/str.h include/blockgame/panic.h \
 include/blockgame/macro.h
utility.o: src/utility.c include/blockgame/utility.h \
 include/blockgame/panic.h include/blockgame/macro.h
vector.o: src/vector.c include/blockgame/vector.h \
 include/blockgame/utility.h
vertex.o: src/vertex.c include/blockgame/vertex.h \
 include/blockgame/mathtypes.h


build: main.o
	$(CC) -g -o$(BLOCKGAME) $(LDFLAGS) $(CFLAGS) src/*.c

format:
	$(CLANG_FORMAT) -i -style=file src/*.c src/*.h include/blockgame/*.h

.PHONY: build