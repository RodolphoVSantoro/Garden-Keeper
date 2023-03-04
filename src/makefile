INC=-I../include -I../include/SDL -I../include/SDL_image -I../include/extension -I../src
LINK=-L../lib -L../bin
WARN=-W
#-Wall
#-Wno-unused -Wno-unused-function -Wno-unused-variable -Wno-unused-value -Wno-unused-label -Wno-unused-parameter -Wno-missing-braces -Wno-missing-field-initializers -Wno-implicit-function-declaration -Wno-return-type
FLAGS=-march=core2 -O2 -ansi -std=gnu++11
LIBS=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
CC=g++
main=main.cpp
output=-o ..\bin\jogo.exe
exec=..\bin\jogo.exe

default: jogo

jogo: $(main)
	$(CC) $(output) $(main) $(INC) $(LINK) $(FLAGS) $(LIBS)

run: $(exec)