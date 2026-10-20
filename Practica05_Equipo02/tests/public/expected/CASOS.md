# Resultados esperados — P05

Las posiciones corresponden a los archivos de `inputs/`, no a los números de
línea que pueda mostrar un editor. Línea inicial 1, columna inicial 0.

## Casos aceptados dentro del alcance de P05

Todos deben terminar con código 0, stderr vacío y stdout igual a
`programa_correcto.out`. No imprimen los valores de `print`, pues P05 no ejecuta
MiniC, ni imprimen el AST.

| Caso | Qué comprueba |
|---|---|
| p01_empty | Archivo de cero bytes; ámbito global sin sentencias. |
| p02_literals | Sentencias sin declaraciones ni usos de variables. |
| p03_declarations | Declaraciones globales de ambos tipos. |
| p04_valid_use | Inicialización, destino y usos en expresión y print. |
| p09_nested_visibility | Búsqueda de variables exteriores en varios niveles. |
| p11_shadowing | Mismo nombre en tres niveles; restauración del ámbito exterior. |
| p12_sibling_scopes | El mismo nombre puede declararse en bloques hermanos. |
| p15_self_initializer | La variable se registra antes de analizar su inicializador. No se evalúa inicialización definida. |
| p18_if_else | Ambas ramas se recorren; sus bloques tienen ámbitos distintos. |
| p19_while | Condición, cuerpo y uso posterior de la variable global. |
| p20_types_deferred | Los nombres se resuelven. La incompatibilidad de tipos y las condiciones enteras corresponden a P06. |
| p24_empty_blocks | Entrada y salida de ámbitos vacíos y hermanos. |

## Casos rechazados

Todos deben terminar con código distinto de cero, stdout vacío y diagnósticos
en stderr. La redacción exacta es libre dentro de lo establecido por el PDF.
La tabla indica los hechos que deben comunicar los diagnósticos; no son textos
literales para comparar con `diff`.

| Caso | Diagnósticos esperados |
|---|---|
| p05_undeclared_assignment | Uso no declarado de `x` en `[1:0]`. |
| p06_use_before_declaration | Uso no declarado de `x` en `[1:0]`; la declaración posterior no lo valida retroactivamente. |
| p07_global_redeclaration | Redeclaración de `x` en `[2:5]`. Primera declaración en `[1:4]`. |
| p08_local_redeclaration | Redeclaración de `x` en `[3:8]`. Primera declaración en `[2:8]`. |
| p10_out_of_scope | Uso no declarado de `x` en `[4:0]`. |
| p13_sibling_visibility | Uso no declarado de `x` en `[5:10]`; pertenece al otro bloque. |
| p14_initializer_use | Uso no declarado de `y` en `[1:8]`. `x` ya está registrado. |
| p16_multiple_errors | Usos no declarados de `x` en `[1:0]`, `y` en `[2:0]` y `w` en `[5:6]`; redeclaración de `z` en `[4:4]`, primera declaración `[3:4]`. |
| p17_condition_uses | Usos no declarados de `flag` en `[1:4]` y `run` en `[2:7]`. |
| p21_syntax_error | Error sintáctico en `[1:8]`: falta expresión antes de `;`. No ejecutar semántica ni diagnosticar semánticamente `missing`. |
| p22_lexical_error | Error léxico por `@` en `[2:0]`. El token ERROR no debe generar además un diagnóstico sintáctico por sí mismo. No ejecutar semántica ni diagnosticar semánticamente `missing`. |
| p23_expression_uses | Usos no declarados de `a` en `[2:5]` y `b` en `[2:9]`; el destino `x` sí está declarado. |

La referencia a la declaración anterior en una redeclaración es recomendable,
no obligatoria. Se incluye aquí para facilitar la revisión.

## Comprobación manual

- ¿Se detectan todos los errores independientes indicados, incluidos ambos operandos?
- ¿La posición señala el identificador implicado y considera la sangría?
- ¿Se conserva el símbolo original después de una redeclaración?
- ¿Se evita confundir un fallo interno con un error semántico?
- ¿Los casos p21 y p22 impiden ejecutar semántica?

La salida normal por sí sola no permite comprobar qué símbolo se resolvió en
todos los casos de shadowing. Complementen con pruebas de su tabla y depuración
fuera de la salida normal, sin añadir verificaciones de tipos a P05.
