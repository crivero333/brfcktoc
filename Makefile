CC      := gcc
CFLAGS  := -Wall -Wextra -pedantic -std=c99 -O2 -g
TARGET  := brfcktoc

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) main.c -o $@

clean:
	rm -f $(TARGET) *.exe

.PHONY: all clean