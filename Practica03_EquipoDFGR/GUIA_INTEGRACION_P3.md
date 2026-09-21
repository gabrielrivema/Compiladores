# Guía de integración — Práctica 3

Esta guía propone un orden de trabajo. No define requisitos adicionales y no
sustituye el enunciado de la práctica ni la especificación de MiniC.

## Antes de comenzar

1. Conserven una copia funcional de la Práctica 2.
2. Ejecuten las pruebas de regresión de P01 y P02.
3. Localicen dónde se crea, conserva, imprime y destruye cada token.
4. Lean por completo la gramática y las reglas de recuperación de P03.
5. Definan y documenten la propiedad de los tokens y sus lexemas.

## Orden sugerido de implementación

### 1. Convertir el lexer en productor incremental

Adapten el lexer para que cada llamada entregue un token y conserve el estado
necesario para la siguiente. Separen la creación del token de su impresión.

Comprueben primero que varias llamadas consecutivas entreguen la misma secuencia
que producía P02, sin escribirla en `stdout`.

### 2. Crear el estado del parser

Incorporen un módulo `parser` separado. Inicialicen el lexer, el token actual, las
banderas de error y cualquier estado de recuperación que utilicen.

### 3. Implementar las operaciones auxiliares

Completen operaciones equivalentes a:

- avanzar al siguiente token;
- consultar el tipo del token actual;
- aceptar opcionalmente un token;
- exigir un token esperado;
- emitir un diagnóstico;
- sincronizarse después de un error.

Verifiquen aquí la propiedad de memoria antes de implementar la gramática completa.

### 4. Reconocer el programa y las sentencias

Implementen primero `program`, la lista de sentencias y la selección por el token
de anticipación. Después agreguen, una por una:

1. declaraciones;
2. asignaciones;
3. `print`;
4. bloques;
5. `if` y `else`;
6. `while`.

Prueben cada producción antes de continuar.

### 5. Implementar expresiones por niveles

Mantengan una función por nivel de precedencia, desde `expression` hasta `primary`.
Prueben primero literales, identificadores y paréntesis; después incorporen el
unario y los operadores binarios nivel por nivel.

### 6. Incorporar diagnósticos y recuperación

Cuando falte un elemento obligatorio, reporten el token encontrado y la posición
correspondiente. Apliquen los tokens de sincronización publicados sin ocultar el
token que debe procesar la estructura exterior.

Comprueben que cada ruta de error consume un token o devuelve el control a una
función capaz de procesarlo.

### 7. Integrar el resultado general

`main.c` debe coordinar la apertura, inicialización, análisis, liberación y código
de salida. Sólo un archivo completamente válido imprimirá:

```text
Programa sintacticamente correcto.
```

### 8. Ejecutar regresión y revisar memoria

Conserven las pruebas léxicas, aunque ahora el modo normal no imprima tokens.
Revisen rutas válidas, errores léxicos, errores sintácticos, recuperación, EOF y
fallos internos.

## Lista de comprobación

- [ ] El lexer entrega tokens incrementalmente y sin imprimirlos.
- [ ] El parser no inspecciona directamente caracteres del archivo.
- [ ] Cada token y lexema tiene un propietario documentado.
- [ ] Existe al menos un token de anticipación.
- [ ] Las funciones del parser corresponden a responsabilidades separadas.
- [ ] Se reconocen todas las sentencias publicadas.
- [ ] Las expresiones respetan precedencia y asociatividad.
- [ ] Un token `ERROR` no genera además un diagnóstico sintáctico por sí mismo.
- [ ] La recuperación no entra en ciclos ni repite indefinidamente un error.
- [ ] Se consume la entrada completa hasta `TOKEN_EOF`.
- [ ] Sólo los programas válidos imprimen el mensaje de éxito.
- [ ] Los diagnósticos se escriben en `stderr`.
- [ ] Los códigos de salida distinguen éxito de programa inválido.
- [ ] Se liberan tokens, lexemas y estados aun cuando haya errores.
- [ ] El proyecto no construye AST ni realiza validaciones semánticas.

