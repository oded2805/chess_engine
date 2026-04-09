CC = gcc
CFLAGS = -Wall -Wextra -std=c17 -Iinclude -O3 -flto -fomit-frame-pointer
LDFLAGS = -flto

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = chess

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS)

clean:
	rm -f src/*.o $(TARGET)
