FLAGS= -std=c99 -Wall -O2 -D_GNU_SOURCE
PKG = `pkg-config --cflags --libs glib-2.0`
CC=gcc
OBJ=obj
SRC = src
LIBS = include

program: output main user commits repos sgg

output: user commits repos main sgg
	$(CC) $(FLAGS) $(OBJ)/user.o $(OBJ)/commits.o $(OBJ)/repos.o $(OBJ)/sgg.o $(OBJ)/main.o $(PKG) -o guiao-2

user: $(SRC)/user.c $(LIBS)/user.h
	$(CC) $(FLAGS) -o $(OBJ)/user.o $(PKG) -c $(SRC)/user.c

commits: $(SRC)/commits.c $(LIBS)/commits.h
	$(CC) $(FLAGS) -o $(OBJ)/commits.o $(PKG) -c $(SRC)/commits.c

repos: $(SRC)/repos.c $(LIBS)/repos.h
	$(CC) $(FLAGS) -o $(OBJ)/repos.o $(PKG) -c $(SRC)/repos.c

sgg: $(SRC)/sgg.c $(LIBS)/sgg.h
	$(CC) $(FLAGS) -o $(OBJ)/sgg.o $(PKG) -c $(SRC)/sgg.c

main: $(SRC)/main.c
	$(CC) $(FLAGS) -o $(OBJ)/main.o $(PKG) -c $(SRC)/main.c

clean:
	rm -rf $(OBJ)/*.o output
