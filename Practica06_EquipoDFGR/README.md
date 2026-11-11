# Práctica 6 — Verificación de Tipos en MiniC

## Información general

| Asignatura | Compiladores |
| Número de práctica | 6 |
| Equipo | DFGR |

## Integrantes

| Nombre | Número de cuenta | Correo electrónico |
| Flores Galeana Daniela | 321162342 | danielafg@ciencias.unam.mx |
| Rivera Machuca Gabriel Eduardo | 321057608 | gabrielrivema@ciencias.unam.mx |

## Estructura del proyecto

```text
Practica06_EquipoDFGR/
├── README.md
├── CHANGELOG.md
├── reporte.tex
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
    │   ├── inputs/
    │   ├── expected/
    │   └── run_public_tests.sh
    ├── equipo/
    │   ├── inputs/
    │   ├── expected/
    │   └── run_tests.sh
    ├── tipos/
    │   ├── inputs/
    │   ├── expected/
    │   └── run_tests.sh
    ├── public_p06/
    │   ├── casos/
    │   ├── esperados/
    │   ├── ejemplos/
    │   ├── cases.txt
    │   ├── README.md
    │   └── ejecutar_pruebas.sh
    └── memoria/
        ├── fail_alloc.c
        └── run_tests.sh
```

### Módulos implementados

| Módulo | Descripción |
| :--- | :--- |
| `src/main.c` | Coordina la apertura del archivo y las etapas del compilador, muestra el resultado y libera los recursos. |
| `src/lexer/lexer.c` e `include/lexer/lexer.h` | Lee el archivo, reconoce los elementos léxicos y registra línea y columna. |
| `src/lexer/token.c` e `include/lexer/token.h` | Define los tokens y administra la memoria de sus lexemas. |
| `src/parser/parser.c` e `include/parser/parser.h` | Analiza la gramática mediante descenso recursivo, construye el AST y maneja errores sintácticos e internos. |
| `src/ast/ast.c` e `include/ast/ast.h` | Define los nodos y las listas del AST, conserva su función de impresión y libera el árbol de forma recursiva. |
| `src/semantic/symbol.c` e `include/semantic/symbol.h` | Crea y destruye símbolos, representa los tipos y proporciona `type_to_string`. |
| `src/semantic/scope.c` e `include/semantic/scope.h` | Administra los símbolos pertenecientes a un ámbito. |
| `src/semantic/symbol_table.c` e `include/semantic/symbol_table.h` | Administra los ámbitos y realiza búsquedas locales y visibles e inserciones de símbolos. |
| `src/semantic/semantic.c` e `include/semantic/semantic.h` | Recorre el AST y comprueba declaraciones, visibilidad, tipos, operadores, asignaciones y condiciones. |

### Verificación de tipos

El analizador reutiliza los ámbitos y la tabla de símbolos de la práctica anterior. Cada expresión devuelve su tipo durante el recorrido del AST: `TYPE_INT`, `TYPE_BOOL` o `TYPE_ERROR`; `TYPE_UNKNOWN` representa un tipo no determinado. Los tipos inferidos no se almacenan en el árbol.

Las inicializaciones y asignaciones requieren tipos iguales, sin conversiones implícitas. Los operadores aritméticos y relacionales requieren operandos enteros; los lógicos requieren booleanos y los de igualdad aceptan operandos del mismo tipo. Las condiciones de `if` y `while` deben ser booleanas. `print` acepta expresiones válidas de ambos tipos.

Los identificadores se resuelven con el símbolo visible más cercano, por lo que se conserva el *shadowing*. `TYPE_ERROR` evita diagnósticos en cascada, mientras se siguen analizando los operandos y sentencias independientes. El compilador no evalúa expresiones ni genera código.

La tabla es dueña de sus ámbitos y símbolos; cada símbolo mantiene una copia de su nombre y una referencia prestada a su declaración. El AST posee sus nodos y cadenas. Los constructores adquieren los hijos solo cuando terminan correctamente; si fallan, el parser libera los recursos que conserva.

## Requisitos

- GCC con soporte para C11.
- GNU Make.
- Bash y las utilidades habituales de Linux (`timeout`, `cmp`, `diff` y `mktemp`) para las pruebas.
- Valgrind para las pruebas de memoria.

## Compilación

```text
make
```

Para eliminar el ejecutable y los archivos generados por la compilación:

```text
make clean
```

## Ejecución

```text
./minic programa.mc
```

Si el programa es válido, `stdout` contiene únicamente `Programa semanticamente correcto.` y el código de salida es 0. Si hay errores, los diagnósticos van a `stderr`, `stdout` permanece vacío y el código de salida es distinto de 0. Si existen errores léxicos o sintácticos, no se ejecuta el análisis semántico.

## Pruebas

Las pruebas se encuentran en `tests/public/` (24 casos heredados), `tests/equipo/` (10 casos), `tests/tipos/` (10 casos), `tests/public_p06/` (24 casos del material oficial) y `tests/memoria/`. Cubren programas válidos, errores léxicos y sintácticos, declaraciones y ámbitos, reglas de tipos, propagación de errores y manejo de memoria.

Las pruebas del equipo y de tipos comparan la salida, el flujo de errores y el código de salida con los resultados esperados. Las pruebas públicas comprueban los canales de forma general; los diagnósticos se describen en `tests/public/expected/CASOS.md`. El caso `p20_types_deferred`, aceptado en la práctica 5, se rechaza ahora que se verifican tipos.

### Ejecución de las pruebas

Para ejecutar las cuatro baterías funcionales, incluidos los casos oficiales de la práctica 6:

```text
make test
```

También pueden ejecutarse por separado:

```text
make test_eq
make test_tipos
sh tests/public/run_public_tests.sh ./minic
make test_p06
```

Los casos, expectativas y ejecutor de `tests/public_p06/` se copiaron del archivo `Material_Practica6.zip` sin modificar sus entradas ni sus criterios. Sus ejemplos están en `tests/public_p06/ejemplos/`. Se revisaron las causas, tipos, posiciones y ausencia de cascadas de los casos inválidos contra `tests/public_p06/esperados/CASOS.md`, además de ejecutar el contrato automático. Este contrato por sí solo no certifica los diagnósticos ni determina la calificación.

Para comprobar fugas en el programa válido:

```text
valgrind --leak-check=full --show-leak-kinds=all ./minic tests/tipos/inputs/valido.mc
```

Para simular fallos de reserva uno por uno sobre ese programa y comprobarlos con Valgrind:

```text
make test_memoria
```

Esta última prueba construye un ejecutable temporal y puede tardar varios minutos. La revisión realizada y sus límites se explican en `reporte.tex`.
