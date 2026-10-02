# Guía para el reporte — Práctica 4

El reporte final deberá entregarse en PDF y explicar la construcción del Árbol de
Sintaxis Abstracta. No debe limitarse a capturas de pantalla ni listados de código.

## Secciones mínimas

### 1. Portada

- asignatura;
- número y nombre de la práctica;
- número de equipo e integrantes;
- semestre;
- fecha de entrega.

### 2. Objetivo

Expliquen qué representa un AST y cuál es su papel entre el parser y las futuras
etapas semánticas.

### 3. Evolución respecto de la Práctica 3

Describan:

- módulos agregados o modificados;
- funciones del parser que ahora devuelven nodos;
- funcionalidades léxicas y sintácticas conservadas;
- refactorizaciones realizadas y su justificación.

### 4. Diseño del AST

Incluyan:

- tipos de nodo;
- información almacenada en cada tipo;
- representación de operadores;
- representación de listas de sentencias;
- representación de campos opcionales;
- información de posición conservada.

### 5. Construcción desde el parser

Expliquen cómo cada producción principal crea su nodo y cómo las funciones de
expresiones conservan la precedencia y asociatividad.

### 6. Propiedad y memoria

Documenten:

- propiedad de nombres y lexemas;
- transferencia de nodos hijos;
- transferencia de nodos a listas;
- comportamiento cuando falla un constructor o una inserción;
- liberación recursiva del árbol;
- tratamiento de árboles parciales.

### 7. Impresión canónica

Describan el recorrido utilizado, la administración de la profundidad y la
correspondencia entre los tipos internos y las etiquetas publicadas.

### 8. Diagnósticos y recuperación

Expliquen cómo conservaron el comportamiento de P03, cómo evitan construir nodos
incompletos y qué ocurre con el AST cuando el archivo contiene errores.

### 9. Resultados y pruebas

Incluyan casos de:

- programa vacío;
- declaraciones y sentencias;
- precedencia y asociatividad;
- condicionales, ciclos y bloques;
- errores léxicos y sintácticos;
- fallos durante la construcción;
- liberación de memoria.

Las pruebas públicas no sustituyen las pruebas diseñadas por el equipo.

### 10. Problemas encontrados

Describan problemas relevantes, soluciones aplicadas y limitaciones conocidas.

### 11. División del trabajo

Expliquen la participación de cada integrante. Ambos deben comprender y poder
explicar la implementación completa.

### 12. Uso de herramientas de Inteligencia Artificial

Cuando se hayan utilizado, indiquen herramienta, propósito, prompt, apoyo recibido
y forma de revisión o comprobación. Si no se utilizaron, indíquenlo explícitamente.

### 13. Conclusiones

Reflexionen sobre lo aprendido y sobre cómo el AST permitirá incorporar la tabla
de símbolos y los ámbitos en la práctica siguiente.

### 14. Referencias

Registren libros, documentación, sitios o código externo consultado de manera
sustantiva.

