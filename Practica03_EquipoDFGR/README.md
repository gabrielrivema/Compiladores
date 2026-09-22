# Práctica 3 — Analizador sintáctico descendente recursivo

## Información general

| Asignatura | Compiladores |
| Número de práctica | 3 |
| Equipo | DFGR |

## Integrantes

| Flores Galeana Daniela | [321162342] | [danielafg@ciencias.unam.mx] |
| Rivera Machuca Gabriel Eduardo | [321057608] | [gabrielrivema@ciencias.unam.mx] |

## Estructura del proyecto

```text
Practica03_EquipoDFGR/
├── README.md
├── CHANGELOG.md
├── Reporte.pdf
├── Makefile
├── src/
│   ├── lexer/
│   │   ├── lexer.c
│   │   └── token.c
│   ├── parser/
│   │   └── parser.c
│   └── main.c
├── include/
│   ├── lexer/
│   │   ├── lexer.h
│   │   └── token.h
│   ├── parser/
│   │   └── parser.h
└── tests/
    ├── public/
    │   ├── expected/
    │   └── inputs/
    └── equipo/
        ├── expected/
        └── inputs/
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

- [x] El lexer entrega tokens incrementalmente y sin imprimirlos.
- [x] El parser no inspecciona directamente caracteres del archivo.
- [x] Cada token y lexema tiene un propietario documentado.
- [x] Existe al menos un token de anticipación.
- [x] Las funciones del parser corresponden a responsabilidades separadas.
- [x] Se reconocen todas las sentencias publicadas.
- [x] Las expresiones respetan precedencia y asociatividad.
- [x] Un token `ERROR` no genera además un diagnóstico sintáctico por sí mismo.
- [x] La recuperación no entra en ciclos ni repite indefinidamente un error.
- [x] Se consume la entrada completa hasta `TOKEN_EOF`.
- [x] Sólo los programas válidos imprimen el mensaje de éxito.
- [x] Los diagnósticos se escriben en `stderr`.
- [x] Los códigos de salida distinguen éxito de programa inválido.
- [x] Se liberan tokens, lexemas y estados aun cuando haya errores.
- [x] El proyecto no construye AST ni realiza validaciones semánticas.


## Pruebas

Las pruebas están divididas en `tests/public/` y `tests/equipo/`. Los casos propios cubren:

- Secuencias de códigos válidas
- Secuencias de códigos inválidas, estas pueden ser por un ';' faltante, un '(' faltante entre otras.

Para ejecutar las pruebas, se debe correr el binario contra los archivos de entrada (`.mc`) y comparar la salida estándar con el contenido de los archivos de salida esperada (`.out`), que están en las carpetas `expected/`.



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
