# Base inicial — Práctica 1

## Qué contiene

- la estructura modular `include/`, `src/` y `tests/`;
- la enumeración completa de categorías requerida por la práctica;
- la estructura `Token`;
- funciones básicas para crear, imprimir y destruir tokens;
- validación de argumentos y apertura del archivo;
- un recorrido inicial del archivo;
- reconocimiento de `PLUS` como ejemplo completo.

## Qué falta implementar

- los demás símbolos simples;
- números enteros;
- seguimiento completo de `\n`, `\r` y `\r\n`;
- generación de `ERROR`;
- recuperación después de caracteres no reconocidos;
- generación y posición de `TOKEN_EOF`;
- validación completa mediante pruebas;
- cualquier ajuste necesario para cumplir exactamente el PDF.

La salida `PENDIENTE` incluida en el lexer solo sirve para señalar trabajo incompleto. No forma parte del formato final permitido.

## Compilación provisional

Mientras se integra el sistema de compilación trabajado en clase, esta base puede comprobarse manualmente con:

```text
gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude \
    src/main.c src/lexer/lexer.c src/lexer/token.c -o minic
```

Después puede ejecutarse con:

```text
./minic archivo.mc
```

La entrega final debe respetar la interfaz de compilación solicitada en la práctica.

## Ruta de trabajo sugerida

1. Ejecutar el programa con un archivo que contenga únicamente `+`.
2. Completar los símbolos restantes en `simple_token_type`.
3. Agregar el reconocimiento de enteros.
4. completar el seguimiento de posición para todos los saltos de línea.
5. Sustituir `PENDIENTE` por tokens `ERROR`.
6. Generar `TOKEN_EOF`.
7. Comparar la salida con las pruebas públicas.
