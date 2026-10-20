# Práctica 5 — Análisis Semántico, Tabla de Símbolos y Ámbitos para MiniC

## Información general

| Asignatura | Compiladores |
| :--- | :--- |
| Número de práctica | 5 |
| Equipo | DFGR |

## Integrantes

| Nombre | Número de Cuenta | Correo Electrónico |
| :--- | :--- | :--- |
| Flores Galeana Daniela | [321162342] | danielafg@ciencias.unam.mx |
| Rivera Machuca Gabriel Eduardo | [321057608] | gabrielrivema@ciencias.unam.mx |

## Estructura del proyecto

```text
Practica05_EquipoDFGR/
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
│   ├── semantic/
│   │   ├── symbol.c
│   │   ├── scope.c
│   │   ├── symbol_table.c
│   │   └── semantic.c
│   └── main.c
├── include/
│   ├── lexer/
│   │   ├── lexer.h
│   │   └── token.h
│   ├── parser/
│   │   └── parser.h
│   ├── ast/
│   │   └── ast.h
│   └── semantic/
│       ├── symbol.h
│       ├── scope.h
│       ├── symbol_table.h
│       └── semantic.h
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
| `src/semantic/symbol.c` e `include/semantic/symbol.h` | Crea y destruye los simbolos individuales tales como variables, tipos o posiciones. |
| `src/semantic/scope.c` e `include/semantic/scope.h` | Gestión de ámbitos individuales y almacenamiento dinámico de símbolos locales. |
| `src/semantic/symbol_table.c` e `include/semantic/symbol_table.h` | Administración de la pila de ámbitos, búsquedas locales y visibles e inseciones con control de duplicados. |
| `src/semantic/semantic.c` e `include/semantic/semantic.h` | Recorrido recursivo del AST para verificar als declaraciones, ámbitos, visibilidad y shadowing. |
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


## Pruebas

Las pruebas están divididas en `tests/public/` y `tests/equipo/`. Los casos propios cubren:

- Secuencias de códigos válidas.
- Secuencias de códigos inválidas, estas pueden ser por un ';' faltante, un '(' faltante entre otras.

Para ejecutar las pruebas, se debe correr el binario contra los archivos de entrada (`.mc`) y comparar la salida estándar con el contenido de los archivos de salida esperada (`.out`), que están en las carpetas `expected/`.

### Ejecución de las pruebas

```text
make test_eq
```