FLAGS= -std=c99 -Wall -O2 -D_GNU_SOURCE -fno-stack-protector
PKG = `pkg-config --cflags --libs glib-2.0`
CC=gcc
OBJ=obj
SRC = src
LIBS = include

program: output output2 validate main user commits valid_date view repos sgg

output2: user commits repos valid_date validate view main sgg test
	$(CC) $(FLAGS) $(OBJ)/user.o $(OBJ)/commits.o $(OBJ)/repos.o $(OBJ)/sgg.o $(OBJ)/validate.o $(OBJ)/valid_date.o $(OBJ)/view.o $(OBJ)/test.o $(PKG) -o tests


output: user commits repos valid_date validate view main sgg
	$(CC) $(FLAGS) $(OBJ)/user.o $(OBJ)/commits.o $(OBJ)/repos.o $(OBJ)/sgg.o $(OBJ)/validate.o $(OBJ)/valid_date.o $(OBJ)/view.o $(OBJ)/main.o $(PKG) -o guiao-3

user: $(SRC)/user.c $(LIBS)/user.h
	$(CC) $(FLAGS) -o $(OBJ)/user.o $(PKG) -c $(SRC)/user.c

commits: $(SRC)/commits.c $(LIBS)/commits.h
	$(CC) $(FLAGS) -o $(OBJ)/commits.o $(PKG) -c $(SRC)/commits.c

repos: $(SRC)/repos.c $(LIBS)/repos.h
	$(CC) $(FLAGS) -o $(OBJ)/repos.o $(PKG) -c $(SRC)/repos.c

sgg: $(SRC)/sgg.c $(LIBS)/sgg.h
	$(CC) $(FLAGS) -o $(OBJ)/sgg.o $(PKG) -c $(SRC)/sgg.c

validate: $(SRC)/validate.c $(LIBS)/validate.h
	$(CC) $(FLAGS) -o $(OBJ)/validate.o $(PKG) -c $(SRC)/validate.c

valid_date: $(SRC)/valid_date.c $(LIBS)/valid_date.h
	$(CC) $(FLAGS) -o $(OBJ)/valid_date.o $(PKG) -c $(SRC)/valid_date.c

view: $(SRC)/view.c $(LIBS)/view.h
	$(CC) $(FLAGS) -o $(OBJ)/view.o $(PKG) -c $(SRC)/view.c


main: $(SRC)/main.c
	$(CC) $(FLAGS) -o $(OBJ)/main.o $(PKG) -c $(SRC)/main.c

test: $(SRC)/tests.c
	$(CC) $(FLAGS) -o $(OBJ)/test.o $(PKG) -c $(SRC)/tests.c


clean:
	rm -rf $(OBJ)/*.o output
