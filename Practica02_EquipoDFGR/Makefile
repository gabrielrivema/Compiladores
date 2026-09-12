CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Iinclude

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
	@for file in tests/public/inputs/*.mc; do \
		if [ -f "$$file" ]; then \
			echo "Probando $$file:"; \
			./$(TARGET) "$$file"; \
			echo "---------------------------"; \
		fi \
	done


test_eq: $(TARGET)
	@echo "Ejecutando pruebas del equipo..."
	@mkdir -p tests
	@for file in tests/equipo/inputs/*.mc; do \
		if [ -f "$$file" ]; then \
			echo "Probando $$file:"; \
			./$(TARGET) "$$file"; \
			echo "---------------------------"; \
		fi \
	done


.PHONY: all clean test
