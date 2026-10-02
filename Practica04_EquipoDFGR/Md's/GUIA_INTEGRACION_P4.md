# Guía de integración — Práctica 4

Esta guía propone un orden de trabajo. No define requisitos adicionales y no
sustituye el enunciado de la práctica ni la especificación de MiniC.

## Antes de comenzar

1. Conserven una copia funcional de la Práctica 3.
2. Ejecuten las pruebas léxicas y sintácticas de regresión.
3. Localicen todas las funciones del parser que reconocen producciones.
4. Revisen quién posee y libera el token de anticipación y su lexema.
5. Lean por completo las reglas de posición, propiedad y salida de P04.

## Orden sugerido de implementación

### 1. Incorporar el módulo `ast`

Adapten `ast.h` y `ast.c` a la organización de su proyecto. Definan tipos de nodo,
operadores, listas y datos asociados sin modificar todavía el comportamiento del
parser.

Comprueben que el proyecto continúe compilando y que las pruebas de P03 sigan
funcionando antes de construir nodos.

### 2. Implementar listas dinámicas

Completen las operaciones de inicialización, crecimiento, inserción y destrucción.
Prueben listas vacías, una inserción, múltiples crecimientos y fallos de reserva.

Documenten en qué momento la lista adquiere la propiedad de un nodo.

### 3. Implementar constructores pequeños

Comiencen con:

- identificadores;
- literales enteros;
- literales booleanos;
- expresiones unarias;
- expresiones binarias.

Después incorporen sentencias, bloques y el nodo programa. Cada constructor debe
inicializar completamente el nodo, copiar las cadenas necesarias y respetar el
contrato de propiedad publicado.

### 4. Modificar las funciones de expresiones

Cambien las funciones del parser para que devuelvan nodos. Mantengan una función
por nivel de precedencia y construyan cada nodo binario a partir del árbol
izquierdo acumulado y el nuevo operando derecho.

Prueben precedencia, asociatividad izquierda, negación unaria y paréntesis antes
de continuar con las sentencias.

### 5. Construir sentencias

Adapten, una por una:

1. declaraciones;
2. asignaciones;
3. `print`;
4. bloques;
5. `if` y `else`;
6. `while`;
7. programa completo.

Después de cada incorporación, verifiquen tanto la aceptación de la entrada como
la forma exacta del árbol producido.

### 6. Conservar la recuperación de errores

Si una producción no puede completarse, no construyan nodos con hijos faltantes.
Liberen los nodos parciales cuya propiedad todavía conserven y permitan que la
estrategia de recuperación de P03 continúe localizando errores posteriores.

Si el archivo completo contiene algún error, destruyan cualquier árbol parcial,
no impriman el AST y terminen con un código distinto de cero.

### 7. Implementar la impresión canónica

Mantengan separada la representación interna del formato de salida. Impriman un
elemento por línea, exactamente dos espacios por nivel y únicamente las etiquetas
publicadas.

Prueben por separado árboles pequeños antes de comparar programas completos.

### 8. Completar la liberación recursiva

Liberen cadenas, hijos, listas, arreglos internos y finalmente el nodo. La función
de destrucción debe aceptar `NULL` y ser segura para todas las clases de nodo.

### 9. Ejecutar regresión y revisar memoria

Ejecuten las pruebas de P01, P02 y P03, las pruebas públicas de P04 y sus propios
casos. Revisen con una herramienta de memoria tanto entradas válidas como
inválidas.

## Lista de comprobación

- [ ] El lexer continúa produciendo tokens sin imprimirlos.
- [ ] El parser conserva sus diagnósticos y recuperación de P03.
- [ ] Cada función principal del parser devuelve el nodo correspondiente.
- [ ] Los lexemas necesarios se copian antes de liberar o reemplazar tokens.
- [ ] Las listas crecen dinámicamente y no imponen un límite fijo pequeño.
- [ ] La precedencia y asociatividad quedan reflejadas en la estructura.
- [ ] Cada nodo conserva la línea y columna indicadas en el enunciado.
- [ ] Los constructores documentan cuándo adquieren la propiedad de sus hijos.
- [ ] Un fallo de construcción libera únicamente los recursos todavía poseídos.
- [ ] La salida coincide exactamente con el formato canónico.
- [ ] Un programa inválido deja `stdout` vacío.
- [ ] `ast_destroy(NULL)` es seguro.
- [ ] Liberar la raíz libera el árbol completo.
- [ ] No se realizan todavía validaciones semánticas.

