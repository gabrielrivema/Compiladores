# Pruebas públicas de P06

Desde la raíz del proyecto ejecuten:
```bash
bash /ruta/Material_Practica6/tests/ejecutar_pruebas.sh ./minic
```
La ruta puede contener espacios. El ejecutor no compila.

cases.txt relaciona entrada y resultado (valid/invalid). Cada caso tiene su
objetivo y expectativas en esperados/CASOS.md. Líneas desde 1, columnas desde 0.

## Comprobación automática

- Válido: código 0, stdout idéntico a esperados/programa_correcto.out,
  stderr vacío.
- Inválido: código no cero, stdout vacío y stderr no vacío.
- Se muestran los diagnósticos inválidos para revisión humana.
- No se cuentan líneas de stderr: un diagnóstico puede ocupar varias líneas.
- No se exige coincidencia textual de diagnósticos ni un código no cero concreto.

La revisión humana debe comprobar causas, posiciones, tipos, errores
independientes y ausencia de cascadas según CASOS.md y PDF.
Aceptar o rechazar por sí solo no acredita la comprobación semántica.

Cada ejecución tiene un límite operativo de 5 segundos. TIMEOUT_S permite
ajustarlo (por ejemplo TIMEOUT_S=20 bash ...). Ese límite no es un requisito
académico de rendimiento. Un timeout se reporta como problema de ejecución,
no como rechazo válido. Bash no permite distinguir de forma general un código
alto elegido por el programa de una terminación por señal; revisen cualquier
mensaje de aborto, segmentación o fallo de lanzamiento. Esos problemas nunca
cuentan como diagnósticos semánticos correctos, aunque pasen el contrato mínimo.

El script devuelve 0 si pasan los contratos automáticos, 1 si hay fallos
observados y 2 si no puede prepararse. No determina una calificación.
