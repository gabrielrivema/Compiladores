CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Iinclude

SRCS = src/main.c src/lexer/lexer.c src/lexer/token.c src/parser/parser.c src/ast/ast.c src/semantic/symbol.c src/semantic/scope.c src/semantic/symbol_table.c src/semantic/semantic.c
OBJS = $(SRCS:.c=.o)
DEPS = $(OBJS:.o=.d)
TARGET = minic

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)

-include $(DEPS)

test: test_public test_eq test_tipos test_p06

test_public: $(TARGET)
	sh tests/public/run_public_tests.sh ./$(TARGET)

test_p06: $(TARGET)
	bash tests/public_p06/ejecutar_pruebas.sh ./$(TARGET)


test_eq: $(TARGET)
	sh tests/equipo/run_tests.sh ./$(TARGET)


.PHONY: all clean test test_public test_p06

test_tipos: $(TARGET)
	sh tests/tipos/run_tests.sh ./$(TARGET)

test_memoria:
	sh tests/memoria/run_tests.sh

.PHONY: test_eq test_tipos test_memoria
