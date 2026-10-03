# Práctica 4 — Árbol de Sintaxis Abstracta (AST) para MiniC

## Información general

| Asignatura | Compiladores |
| :--- | :--- |
| Número de práctica | 4 |
| Equipo | DFGR |

## Integrantes

| Nombre | Número de Cuenta | Correo Electrónico |
| :--- | :--- | :--- |
| Flores Galeana Daniela | [321162342] | danielafg@ciencias.unam.mx |
| Rivera Machuca Gabriel Eduardo | [321057608] | gabrielrivema@ciencias.unam.mx |

## Estructura del proyecto

```text
Practica04_EquipoDFGR/
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
│   ├── ast/
│   │   └── ast.c
│   └── main.c
├── include/
│   ├── lexer/
│   │   ├── lexer.h
│   │   └── token.h
│   ├── parser/
│   │   └── parser.h
│   └── ast/
│       └── ast.h
└── tests/
    ├── public/
    │   ├── expected/
    │   └── inputs/
    └── equipo/
        ├── expected/
        └── inputs/
```

### Módulos implementados
|Modulo | Descripción |
|:-----|:------
| `src/main.c` | Punto de entrada, inicia el flujo, invoca al parser e imprime el AST, finalmente libera los recursos del sistema. |
| `src/lexer/lexer.c` e `include/lexer/lexer.h` | Lógica del autómata, lectura de caracteres del archivo, seguimiento preciso de línea/columna e ignorado de espacios en blanco. |
| `src/lexer/token.c` e `include/lexer/token.h` | Definición de las categorías léxicas , creación de la estructura del token, manejo de memoria dinámica de los lexemas y su posterior liberación. |
| `src/parser/parser.c` e `include/parser/parser.h` | Consume los tokens generados por lexeme mediante descenso recursivo, además válida a grámatica, construir los nodos del AST y aplicar recuperación de errores. |
| `src/ast/ast.c` e `include/ast/ast.h` | Define la estrucura del AST, gestiona listas, imprime el AST y libera la memoria de manera recursiva. |

## Requisitos

- GCC con soporte para C11
- GNU Make

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


- [x] El lexer continúa produciendo tokens sin imprimirlos.
- [x] El parser conserva sus diagnósticos y recuperación de P03.
- [x] Cada función principal del parser devuelve el nodo correspondiente.
- [x] Los lexemas necesarios se copian antes de liberar o reemplazar tokens.
- [x] Las listas crecen dinámicamente y no imponen un límite fijo pequeño.
- [x] La precedencia y asociatividad quedan reflejadas en la estructura.
- [x] Cada nodo conserva la línea y columna indicadas en el enunciado.
- [x] Los constructores documentan cuándo adquieren la propiedad de sus hijos.
- [x] Un fallo de construcción libera únicamente los recursos todavía poseídos.
- [x] La salida coincide exactamente con el formato canónico.
- [x] Un programa inválido deja `stdout` vacío.
- [x] `ast_destroy(NULL)` es seguro.
- [x] Liberar la raíz libera el árbol completo.
- [x] No se realizan todavía validaciones semánticas.




## Pruebas

Las pruebas están divididas en `tests/public/` y `tests/equipo/`. Los casos propios cubren:

- Secuencias de códigos válidas
- Secuencias de códigos inválidas, estas pueden ser por un ';' faltante, un '(' faltante entre otras.

Para ejecutar las pruebas, se debe correr el binario contra los archivos de entrada (`.mc`) y comparar la salida estándar con el contenido de los archivos de salida esperada (`.out`), que están en las carpetas `expected/`.

### Ejecución de las pruebas

```text
make test_eq
```