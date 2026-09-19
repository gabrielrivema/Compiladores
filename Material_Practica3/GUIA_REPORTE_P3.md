# Guía para el reporte — Práctica 3

El reporte final deberá entregarse en PDF y explicar la integración del analizador
sintáctico. No debe limitarse a capturas de pantalla ni listados de código.

## Secciones mínimas

### 1. Portada

- asignatura;
- número y nombre de la práctica;
- número de equipo e integrantes;
- semestre;
- fecha de entrega.

### 2. Objetivo

Expliquen qué valida el parser y cuál es su papel entre el lexer y la futura
construcción del AST.

### 3. Evolución respecto de la Práctica 2

Describan:

- módulos modificados o agregados;
- interfaz incremental incorporada al lexer;
- eliminación de la impresión de tokens como efecto secundario;
- refactorizaciones realizadas y su justificación;
- funcionalidades léxicas conservadas.

### 4. Arquitectura e interfaz

Incluyan:

- organización del módulo `parser`;
- representación de su estado y token de anticipación;
- responsabilidades del lexer, parser y `main.c`;
- convenio de propiedad y liberación de tokens y lexemas;
- tratamiento de fallos internos del lexer.

### 5. Descenso recursivo y gramática

Expliquen la correspondencia entre producciones y funciones, la selección de
sentencias y la implementación de declaraciones, asignaciones, impresión,
condicionales, ciclos y bloques.

### 6. Expresiones

Describan cómo la estructura de sus funciones conserva la precedencia y
asociatividad publicadas, incluido el operador unario y los paréntesis.

### 7. Diagnósticos y recuperación

Expliquen:

- cuándo se emite un error sintáctico;
- cómo se diferencia de un token léxico `ERROR`;
- cuáles son sus puntos de sincronización;
- cómo garantizan el progreso y evitan ciclos infinitos;
- cómo verifican el consumo completo del archivo.

### 8. Resultados y pruebas

Incluyan casos válidos, inválidos, límite, múltiples errores y regresión del
lexer. Presenten evidencia suficiente para justificar el cumplimiento. Las
pruebas públicas no sustituyen las pruebas diseñadas por el equipo.

### 9. Problemas encontrados

Describan problemas relevantes, soluciones aplicadas y limitaciones conocidas.

### 10. División del trabajo

Expliquen la participación de cada integrante. Ambos deben comprender y poder
explicar la implementación completa.

### 11. Uso de herramientas de Inteligencia Artificial

Cuando se hayan utilizado, indiquen herramienta, propósito, prompt, apoyo recibido
y forma de revisión o comprobación. Si no se utilizaron, indíquenlo explícitamente.

### 12. Conclusiones

Reflexionen sobre lo aprendido y sobre cómo podrá extenderse el parser para
construir el AST en la siguiente práctica.

### 13. Referencias

Registren libros, documentación, sitios o código externo consultado de manera
sustantiva.

