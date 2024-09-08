CC = gcc
CFLAGS = -Iinclude -I"C:\Users\jhans\c_projects\libs\concord\include"
LDFLAGS = -L"C:\Users\jhans\c_projects\libs\concord\lib" -pthread -ldiscord -lcurl -lws2_32

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = .

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
EXECUTABLE = $(BIN_DIR)/myBot.exe

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(EXECUTABLE)
