# Pruebas públicas — Práctica 4

Estas pruebas comprueban una parte mínima de la especificación. No sustituyen las
pruebas que cada equipo debe diseñar.

## Ejecución

Desde la raíz del proyecto:

```text
sh tests/public/run_public_tests.sh
```

También puede indicarse la ruta del ejecutable:

```text
sh tests/public/run_public_tests.sh ./minic
```

## Qué verifican

Para programas válidos se comprueba:

- código de salida cero;
- `stderr` vacío;
- coincidencia exacta de `stdout` con el formato canónico publicado.

Para programas inválidos se comprueba:

- código de salida distinto de cero;
- `stdout` vacío;
- al menos un diagnóstico en `stderr`.

El caso léxico incluido comprueba además que el token `ERROR` no se duplique por
sí mismo como un diagnóstico sintáctico.

## Alcance

El conjunto cubre programa vacío, declaraciones, inicializadores, asignación,
impresión, precedencia, negación, paréntesis, condicionales, ciclos, bloques y dos
clases de error. La evaluación puede utilizar otros casos que se encuentren dentro
de los requisitos publicados.

Si la organización de su proyecto es diferente, pueden copiar `tests/public/` a
la ubicación equivalente o pasar al script la ruta correcta del ejecutable.

