CC = gcc
CFLAGS = -Wall -Wextra -std=c17 -Iinclude -O3 -flto -fomit-frame-pointer
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
TARGET = chess
all: $(TARGET)
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)
%.o: %.c
	$(CC) $(CFLAGS) -c $<
clean:
	rm -f *.o $(TARGET)