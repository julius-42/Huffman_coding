CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2

TARGET = huffman

OBJS = huffman.o structs.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

huffman.o: huffman.c structs.h
	$(CC) $(CFLAGS) -c huffman.c -o huffman.o

structs.o: structs.c structs.h
	$(CC) $(CFLAGS) -c structs.c -o structs.o


run: $(TARGET)
	./$(TARGET)


clean:
	rm -f $(OBJS) $(TARGET)