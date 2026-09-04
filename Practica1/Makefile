CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -std=c11

SRCS = src/main.c src/lexer/lexer.c src/lexer/token.c
OBJS = $(SRCS:.c=.o)
TARGET = minic

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

test: $(TARGET)
	@echo "Ejecutando pruebas..."
	@mkdir -p tests
	@for file in tests/*.mc; do \
		if [ -f "$$file" ]; then \
			echo "Probando $$file:"; \
			./$(TARGET) "$$file"; \
			echo "---------------------------"; \
		fi \
	done

.PHONY: all clean test