# Contrato de referencia — Parser y AST

Este documento resume la integración solicitada por la Práctica 4. Es una
referencia adaptable: no agrega requisitos ni sustituye el enunciado.

## Flujo general

```text
lexer -> Token -> parser -> ASTNode
```

El lexer continúa produciendo tokens mediante la interfaz incremental de P03. El
parser consume esos tokens, reconoce las producciones y solicita al módulo `ast`
la creación de nodos.

## Responsabilidades del parser

El parser debe:

- conservar al menos un token de anticipación;
- reconocer la gramática completa de MiniC;
- decidir qué tipo de nodo corresponde a cada producción;
- pasar a los constructores la posición indicada en el enunciado;
- copiar al AST, mediante sus constructores, la información que deba sobrevivir;
- conservar los diagnósticos y la recuperación implementados en P03;
- liberar nodos parciales cuando una producción no pueda completarse;
- devolver el nodo raíz únicamente cuando el archivo completo sea válido.

El parser no debe implementar dentro de `parser.c` la administración completa de
los nodos, su impresión o su destrucción recursiva.

## Responsabilidades del módulo AST

El módulo `ast` debe:

- definir los tipos y datos de los nodos;
- administrar listas dinámicas de sentencias;
- construir nodos completamente inicializados;
- copiar nombres y lexemas que necesite conservar;
- imprimir el árbol mediante el formato canónico;
- destruir recursivamente el árbol.

## Cadenas y tokens

Los tokens mantienen el convenio de propiedad documentado por el equipo en P03.
Antes de liberar o reemplazar un token, el parser debe entregar al constructor la
información necesaria. El AST debe conservar una copia propia de los nombres de
identificadores y de los lexemas enteros.

El AST no debe guardar apuntadores hacia lexemas temporales del token actual.

## Transferencia de nodos hijos

Para el esqueleto se utiliza el convenio publicado:

- si un constructor devuelve un nodo válido, éste adquiere la propiedad de los
  hijos recibidos;
- si devuelve `NULL`, los hijos continúan perteneciendo al código que realizó la
  llamada y éste debe liberarlos.

La función que llama al constructor debe conservar los apuntadores originales
hasta conocer el resultado de la operación.

## Transferencia a listas

- si `ast_node_list_append` termina correctamente, la lista adquiere la propiedad
  del nodo agregado;
- si falla, la propiedad permanece en el código que realizó la llamada;
- destruir una lista libera sus nodos, su arreglo interno y la deja vacía.

Cuando un nodo `Program` o `Block` se construya correctamente a partir de una
lista, ese nodo será propietario de la lista. Si el constructor falla, el código
que realizó la llamada conservará la responsabilidad de destruirla.

## Errores de construcción

Una producción incompleta no debe originar un nodo aparentemente válido. Ante un
fallo:

1. conserven el estado de error del parser;
2. liberen los nodos cuya propiedad no se haya transferido;
3. apliquen la recuperación sintáctica de P03;
4. continúen el análisis cuando sea posible;
5. destruyan cualquier árbol parcial antes de terminar.

## Resultado general

Un archivo válido produce una raíz `Program`, imprime el AST y termina con código
cero. Un archivo con errores léxicos o sintácticos no entrega el AST a etapas
posteriores, deja `stdout` vacío y termina con un código distinto de cero.

