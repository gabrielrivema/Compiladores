# Guía para el reporte — Práctica 2

El reporte final deberá entregarse en PDF y explicar la extensión realizada al
analizador léxico. No debe limitarse a capturas de pantalla ni a listados de
código.

## Secciones mínimas

### 1. Portada

- asignatura;
- número y nombre de la práctica;
- número de equipo e integrantes;
- semestre;
- fecha de entrega.

### 2. Objetivo

Expliquen brevemente qué funcionalidades incorporaron y cómo estas completan el
analizador léxico de MiniC.

### 3. Evolución respecto de la Práctica 1

Describan:

- módulos modificados o agregados;
- cambios realizados a la representación de tokens;
- correcciones efectuadas a la implementación anterior;
- refactorizaciones relevantes y su justificación.

### 4. Descripción de la solución

Expliquen, como mínimo:

- reconocimiento de identificadores;
- distinción entre identificadores, palabras reservadas y booleanos;
- estrategia de anticipación para operadores compuestos;
- distinción entre `SLASH` y comentarios;
- actualización de línea y columna dentro de comentarios;
- generación de tokens `ERROR` y recuperación;
- generación y posición de `TOKEN_EOF`;
- administración de memoria relevante.

### 5. Resultados y pruebas

Incluyan:

- casos correctos;
- casos límite;
- casos de error;
- pruebas de regresión de la Práctica 1;
- resultados relevantes y evidencia suficiente para justificar el cumplimiento.

Las pruebas públicas no sustituyen las pruebas diseñadas por el equipo.

### 6. Problemas encontrados

Describan los problemas relevantes encontrados, la forma en que se resolvieron y
las limitaciones conocidas que permanezcan en la entrega.

### 7. División del trabajo

Expliquen la participación de cada integrante. Ambos deben comprender y poder
explicar la implementación completa.

### 8. Uso de herramientas de Inteligencia Artificial

Cuando se hayan utilizado, indiquen:

- herramienta;
- propósito;
- prompt utilizado;
- tipo de apoyo recibido;
- forma en que la respuesta fue revisada, comprobada o adaptada.

Si no utilizaron estas herramientas, indíquenlo explícitamente.

### 9. Conclusiones

Reflexionen sobre el resultado, lo aprendido y la utilidad del lexer como entrada
para las siguientes etapas del compilador.

### 10. Referencias

Registren libros, documentación, sitios o código externo consultado de manera
sustantiva.

