# Taller 1 — TextStats reducido

## Alumno
Rivera Machuca Gabriel Eduardo - 321057608

## TODO's

Completar cuatro TODOs en `src/textstats.c`:

1. `text_length`; Recorre text hasta que encuentre la secuencia de escape '\0'
2. `classify_char`; Clasifica el char en letras a-z y A-Z, digitos 0-9 y cualquier secuencia de espacio.
3. crecimiento del buffer dentro de `buffer_append`; Se amplia el espacio del buffer cuando este se llena.
4. recorrido del archivo dentro de `analyze_file`. Mientras no se reciba un EOF se lee el documento y se clasifican los chars.

El programa cuenta caracteres, letras, dígitos, blancos y otros bytes. También conserva una copia dinámica del contenido para hacer visible el uso de `realloc`.

## Comandos

```text
make
./textstats examples/entrada.txt
make test
make clean
```

