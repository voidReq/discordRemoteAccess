CC = gcc
CFLAGS = -Iinclude -I"C:\Users\jhans\programming\c_projects\libs\concord\include"
LDFLAGS = -L"C:\Users\jhans\programming\c_projects\libs\concord\lib" -pthread -ldiscord -lcurl -lws2_32 -mwindows -lurlmon

SRC = src/main.c src/bacon.c src/game.c
OBJ = obj/main.o obj/bacon.o obj/game.o

all: bigOunce.exe

bigOunce.exe: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

obj/%.o: src/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f obj/*.o bigOunce.exe
