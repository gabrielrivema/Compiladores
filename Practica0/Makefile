CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -g
CPPFLAGS = -Iinclude
TARGET = textstats
SOURCES = src/main.c src/textstats.c
BUILD_DIR = build

.PHONY: all test clean

all: $(TARGET)

$(TARGET): $(SOURCES) include/textstats.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SOURCES) -o $(TARGET)

test: $(TARGET)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) tests/test_textstats.c src/textstats.c -o $(BUILD_DIR)/test_textstats
	./$(BUILD_DIR)/test_textstats

clean:
	$(RM) $(TARGET) $(TARGET).exe
	$(RM) -r $(BUILD_DIR)
