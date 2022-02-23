FLAGS= -std=c99 -Wall -O2 -D_GNU_SOURCE
PKG = `pkg-config --cflags --libs glib-2.0`
CC=gcc
OBJ=obj
SRC = src
LIBS = libs

program: output main lerFicheiro user commits repos valid_date

output: lerFicheiro user commits repos valid_date main
	$(CC) $(FLAGS) $(OBJ)/lerFicheiro.o $(OBJ)/user.o $(OBJ)/commits.o $(OBJ)/repos.o $(OBJ)/valid_date.o $(OBJ)/main.o $(PKG) -o guiao-1


lerFicheiro: $(SRC)/lerFicheiro.c $(LIBS)/lerFicheiro.h
	$(CC) $(FLAGS) -o $(OBJ)/lerFicheiro.o $(PKG) -c $(SRC)/lerFicheiro.c

user: $(SRC)/user.c $(LIBS)/user.h
	$(CC) $(FLAGS) -o $(OBJ)/user.o $(PKG) -c $(SRC)/user.c

commits: $(SRC)/commits.c $(LIBS)/commits.h
	$(CC) $(FLAGS) -o $(OBJ)/commits.o $(PKG) -c $(SRC)/commits.c

repos: $(SRC)/repos.c $(LIBS)/repos.h
	$(CC) $(FLAGS) -o $(OBJ)/repos.o $(PKG) -c $(SRC)/repos.c

valid_date: $(SRC)/valid_date.c $(LIBS)/valid_date.h
	$(CC) $(FLAGS) -o $(OBJ)/valid_date.o $(PKG) -c $(SRC)/valid_date.c

main: $(SRC)/main.c
	$(CC) $(FLAGS) -o $(OBJ)/main.o $(PKG) -c $(SRC)/main.c

clean:
	rm -rf $(OBJ)/*.o output
