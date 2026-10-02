# Material de apoyo — Práctica 4

Este paquete complementa el enunciado de la Práctica 4 y la definición vigente
de MiniC. No sustituye ninguno de esos documentos.

## Punto de partida

La Práctica 4 extiende el compilador desarrollado hasta la Práctica 3. Cada equipo
deberá trabajar sobre su lexer y parser funcionales, conservar sus diagnósticos y
mantener la estrategia de recuperación ya implementada.

El directorio `base-inicial/` no es un proyecto completo ni una solución. Contiene
tipos, declaraciones y funciones incompletas de referencia que deben adaptarse a
la arquitectura, nombres y decisiones de propiedad de memoria de cada equipo.

## Contenido

- `GUIA_INTEGRACION_P4.md`: orden sugerido para incorporar el AST.
- `CONTRATO_PARSER_AST.md`: responsabilidades y propiedad entre parser y AST.
- `GUIA_REPORTE_P4.md`: contenido mínimo recomendado para el reporte.
- `base-inicial/`: interfaz y esqueleto incompleto del módulo `ast`.
- `tests/public/`: pruebas públicas, resultados esperados y ejecutor.

## Importante

- No comiencen un proyecto nuevo ni sustituyan su lexer o parser por estos archivos.
- La base no incluye lexer, parser, `Makefile` ni `CHANGELOG.md`.
- Los nombres propuestos pueden adaptarse sin cambiar el comportamiento exigido.
- Los `TODO` forman parte del trabajo de la práctica.
- Las pruebas públicas son mínimas y no reemplazan las pruebas del equipo.
- Un programa inválido no deberá imprimir un AST parcial.
- Durante la evaluación podrán utilizarse casos adicionales que respeten la
  especificación publicada.

