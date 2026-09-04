# Taller 1 — TextStats reducido

## Meta de los primeros 50 minutos

Completar cuatro TODOs en `src/textstats.c`:

1. `text_length`;
2. `classify_char`;
3. crecimiento del buffer dentro de `buffer_append`;
4. recorrido del archivo dentro de `analyze_file`.

El programa cuenta caracteres, letras, dígitos, blancos y otros bytes. También conserva una copia dinámica del contenido para hacer visible el uso de `realloc`.

## Comandos

```text
make
./textstats examples/entrada.txt
make test
make clean
```

