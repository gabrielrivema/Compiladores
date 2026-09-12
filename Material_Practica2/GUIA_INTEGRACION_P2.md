# Guía de integración — Práctica 2

Esta guía propone un orden de trabajo. No define requisitos adicionales y no
sustituye el enunciado de la práctica ni la especificación de MiniC.

## Antes de comenzar

1. Conserven una copia funcional de la entrega de la Práctica 1.
2. Ejecuten nuevamente sus pruebas de la Práctica 1.
3. Verifiquen que `make`, `make clean` y `./minic archivo.mc` funcionen.
4. Revisen que los tokens conserven tipo, lexema, línea y columna iniciales.
5. Lean por completo el enunciado de la Práctica 2 y la definición de MiniC.

## Orden sugerido de implementación

### 1. Revisar las categorías de tokens

Comprueben que su enumeración y la conversión de categorías a texto contemplen
todos los tokens exigidos por MiniC. No cambien libremente sus nombres.

### 2. Reconocer identificadores

Implementen el patrón publicado para identificadores y consuman todos los
caracteres que formen parte de un mismo lexema.

### 3. Reclasificar palabras reservadas

Después de reconocer el lexema completo de un identificador, determinen si
corresponde a una palabra reservada o a un literal booleano. Recuerden que MiniC
distingue mayúsculas y minúsculas.

### 4. Incorporar operadores compuestos

Utilicen anticipación de un carácter para distinguir, entre otros casos, `=` de
`==` y `<` de `<=`. Consuman solamente los caracteres que pertenezcan al operador
reconocido.

### 5. Distinguir división y comentarios

Reconozcan `/` como `SLASH`, excepto cuando el carácter siguiente también sea `/`.
Los comentarios no producen tokens, pero sí deben actualizar correctamente la
posición.

### 6. Completar el manejo de errores

Cada carácter no reconocido debe producir su propio token `ERROR`. Después de
emitirlo, el lexer debe continuar con el carácter siguiente.

### 7. Comprobar el final del archivo

Generen exactamente un `TOKEN_EOF` con la posición posterior al último carácter
procesado, incluso cuando la entrada termine con espacios o comentarios.

### 8. Ejecutar pruebas de regresión

Ejecuten primero las pruebas conservadas de la Práctica 1 y después las pruebas
públicas de la Práctica 2. Una funcionalidad nueva no debe romper símbolos
simples, enteros, posiciones ni recuperación básica.

## Integración recomendada

- Eviten reescribir el proyecto completo.
- Mantengan `main.c` limitado a la coordinación general.
- Conserven el reconocimiento léxico dentro de sus módulos.
- Agreguen funciones auxiliares pequeñas cuando una responsabilidad pueda
  aislarse con claridad.
- Documenten en `CHANGELOG.md` las modificaciones relevantes.

## Lista de comprobación

- [ ] Las pruebas de la Práctica 1 continúan aprobándose.
- [ ] Los identificadores utilizan máxima coincidencia.
- [ ] Las palabras reservadas se clasifican después de reconocer el lexema.
- [ ] Los operadores compuestos se reconocen antes que los simples.
- [ ] `!`, `&` y `|` aislados producen `ERROR`.
- [ ] `/` y `//` se distinguen correctamente.
- [ ] Los comentarios no producen tokens.
- [ ] Los errores no detienen el análisis.
- [ ] Línea y columna se conservan correctamente.
- [ ] Se produce exactamente un `TOKEN_EOF`.
- [ ] La salida estándar coincide exactamente con el formato publicado.
- [ ] El proyecto compila con las opciones mínimas exigidas.

