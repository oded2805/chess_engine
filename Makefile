CC = gcc
CFLAGS = -Wall -Wextra -std=c17 -Iinclude -O3 -flto -fomit-frame-pointer
LDFLAGS = -flto

SRC_DIR = src
OBJ_DIR = obj
TARGET = chess

# Find all .c files (board.c, search.c, etc.)
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

all: $(TARGET)

# This is the "Rule" that was missing
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# The | symbol makes $(OBJ_DIR) an "order-only" dependency.
# It ensures the folder exists BEFORE we try to put .o files in it.
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)