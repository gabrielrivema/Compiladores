# Pruebas públicas — P05

Son 24 casos mínimos del alcance publicado. No sustituyen las pruebas propias,
la revisión del código ni las pruebas de memoria.

## Uso

Compilen su proyecto. Desde su raíz:

```text
sh tests/public/run_public_tests.sh ./minic
```

Se requiere un entorno con `sh`, `diff` y `mktemp`, como el utilizado en las
prácticas anteriores (por ejemplo WSL). El ejecutor no compila ni modifica el
proyecto. Su primer argumento es la ruta al ejecutable. Usa un directorio temporal
y elimina sus archivos al terminar. Si `timeout` está disponible, limita cada
ejecución a 10 segundos como protección local; ese tiempo no es un criterio
académico de rendimiento. Puede ajustarse con `P5_TEST_TIMEOUT=30`.

## Qué se comprueba automáticamente

- Casos válidos para P05: código 0, stderr vacío y stdout idéntico a
  `expected/programa_correcto.out`, incluido el salto de línea final.
- Casos inválidos: código no cero, stdout vacío y stderr no vacío.
- Una ejecución interrumpida por el timeout se considera un fallo de ejecución.
  No se exige un código no cero específico al compilador.

Para cada caso inválido se muestran los diagnósticos y la indicación de revisión
manual. El script no impone una frase, acentos, separadores ni orden textual de
diagnósticos que el PDF no haya fijado de manera obligatoria.

**Aprobar las comprobaciones automáticas de un caso inválido no demuestra que
se haya detectado el error correcto.** Comparen los diagnósticos con
`expected/CASOS.md`: categoría, identificador, posición, causa y errores adicionales.
En particular, un mensaje de error interno no satisface un caso semántico inválido.
Una terminación anormal también requiere revisión: las comprobaciones mínimas
no certifican que cualquier código no cero proceda de una recuperación correcta.

El script devuelve 0 si pasan sus comprobaciones mínimas, 1 si alguna falla y
2 si no puede iniciar por un problema de uso o del entorno. No son códigos
obligatorios del compilador ni representan una calificación.

## Alcance y límites

`cases.txt` enumera las entradas y su clasificación. Se conserva un archivo
realmente vacío en `p01_empty.mc`. Las posiciones esperadas usan líneas desde 1,
columnas desde 0 y cuatro espacios donde hay sangría.

Los casos p21 y p22 prueban que los errores anteriores impidan iniciar semántica:
no debe reportarse un error semántico para `missing`. Esta condición se revisa
manualmente. No se simulan fallos de memoria ni se prueba toda la arquitectura.
La conservación del símbolo original tras una redeclaración requiere también
pruebas de la tabla o inspección; no basta con la salida del ejecutable.

El conjunto incluye construcciones que P05 acepta y que P06 rechazará por tipos.
No ejecuten los programas fuente como si fueran un intérprete: solo se analizan.
