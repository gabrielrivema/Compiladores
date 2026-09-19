# Práctica 2 — Analizador léxico completo de MiniC

## Información general

| Asignatura | Compiladores |
| Número de práctica | 2 |
| Equipo | DFGR |

## Integrantes

| Flores Galeana Daniela | [321162342] | [danielafg@ciencias.unam.mx] |
| Rivera Machuca Gabriel Eduardo | [321057608] | [gabrielrivema@ciencias.unam.mx] |

## Estructura del proyecto

```text
Practica2/
├── README.md
├── CHANGELOG.md
├── Reporte.pdf
├── Makefile
├── src/
│   ├── lexer/
│   │   ├── lexer.c
│   │   └── token.c
│   └── main.c
├── include/
│   └── lexer/
│       ├── lexer.h
│       └── token.h
└── tests/
    ├── public/
        ├── expected/
        ├── inputs/
    └── equipo/
        ├── expected/
        ├── inputs/
```

### Módulos implementados

| `src/main.c` | Punto de entrada, validación de E/S, solicitud secuencial de tokens y formateo de impresión. |
| `src/lexer/lexer.c` e `include/lexer/lexer.h` | Lógica del autómata, lectura de caracteres del archivo, seguimiento preciso de línea/columna e ignorado de espacios en blanco. |
| `src/lexer/token.c` e `include/lexer/token.h` | Definición de las categorías léxicas , creación de la estructura del token, manejo de memoria dinámica de los lexemas y su posterior liberación. |

## Requisitos

- GCC con soporte para C11
- GNU Make
- Dependencias adicionales: ninguna

## Compilación

```text
make
```

Para eliminar los archivos generados:

```text
make clean
```

## Ejecución

```text
./minic programa.mc
```

## Funcionalidades implementadas

- [x] Lectura del archivo fuente.
- [x] Sistema de tokens.
- [x] Seguimiento de línea y columna.
- [x] Manejo de espacios en blanco.
- [x] Símbolos simples y compuestos.
- [x] Números enteros.
- [x] Palabras reservadas.
- [x] Caracteres no reconocidos mediante `ERROR`.
- [x] Final del archivo mediante `TOKEN_EOF`.

## Pruebas

Las pruebas están divididas en `tests/public/` y `tests/equipo/`. Los casos propios cubren:

- Manejo de varios saltos de línea de diferentes sistemas operativos: (`\n`, `\r\n`, `\r`).
- Secuencias de múltiples chars no reconocidos seguidos (para asegurar que el lexer se recupera sin abortar y genera varios tokens `ERROR`)
- Reconocimiento continuo de int separados y combinados con símbolos

Para ejecutar las pruebas, se debe correr el binario contra los archivos de entrada (`.mc`) y comparar la salida estándar con el contenido de los archivos de salida esperada (`.out`), que están en las carpetas `expected/`.


## Notas de ejecución

El analizador imprime los tokens reconocidos directamente en stdout con el formato `línea:columna TIPO_TOKEN lexema`. Los errores relacionados con la apertura de archivos o falta de argumentos en la línea de comandos se envían a stderr para evitar contaminar la secuencia de tokens.


## Lista de comprobación

- [x] Las pruebas de la Práctica 1 continúan aprobándose.
- [x] Los identificadores utilizan máxima coincidencia.
- [x] Las palabras reservadas se clasifican después de reconocer el lexema.
- [x] Los operadores compuestos se reconocen antes que los simples.
- [x] `!`, `&` y `|` aislados producen `ERROR`.
- [x] `/` y `//` se distinguen correctamente.
- [x] Los comentarios no producen tokens.
- [x] Los errores no detienen el análisis.
- [x] Línea y columna se conservan correctamente.
- [x] Se produce exactamente un `TOKEN_EOF`.
- [x] La salida estándar coincide exactamente con el formato publicado.
- [x] El proyecto compila con las opciones mínimas exigidas.
