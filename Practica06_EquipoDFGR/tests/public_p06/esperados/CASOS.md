# Expectativas por caso

Todos los casos valid: stdout exacto programa_correcto.out, stderr vacío, código 0.
Todos los casos invalid: stdout vacío, stderr con los diagnósticos descritos,
código distinto de 0. La redacción no tiene que coincidir literalmente.
Las posiciones de operadores y nombres se indican como referencia del token;
para inicializaciones se permiten las alternativas de §14. No se impone un
orden entre diagnósticos independientes si es determinista.

## p01_inicializacion_valida — valid

Inicialización int y bool. Sin diagnósticos.

## p02_inicializacion_invalida — invalid

Dos incompatibilidades de inicialización, líneas 1 y 2. Posición: identificador, = o inicio del inicializador, según política consistente (§14).

## p03_asignacion_valida — valid

Asignaciones compatibles.

## p04_asignacion_invalida — invalid

Dos incompatibilidades de asignación, líneas 3 y 4; destinos en columna 0 como el ejemplo §15.

## p05_aritmetica_valida — valid

Todos los operadores aritméticos y negativo unario. --x son dos negativos, no decremento.

## p06_aritmetica_invalida — invalid

Cinco errores de operadores: + [1:11], - [2:12], * [3:8], / [4:12], negativo [5:6]. Sin errores adicionales de print.

## p07_relacionales_validos — valid

Cada comparación produce bool.

## p08_relacionales_invalidos — invalid

Cuatro errores: < [1:11], <= [2:8], > [3:11], >= [4:12].

## p09_igualdad_valida — valid

Igualdad y diferencia con tipos iguales; resultado bool.

## p10_igualdad_invalida — invalid

Dos errores: == [1:8] y != [2:12].

## p11_logicos_validos — valid

&& y || con booleanos.

## p12_logicos_invalidos — invalid

Dos errores: && [1:8] y || [2:12].

## p13_control_valido — valid

Condiciones bool y print de ambos tipos; no ejecutar el programa.

## p14_control_invalido — invalid

Cuatro errores independientes: condición [1:4], + [1:20], condición [2:7], negativo [2:18].

## p15_ramas_no_ejecutadas — invalid

Tres errores: ausente [1:19], negativo [1:44], + [2:24]. Se revisan ambas ramas y el cuerpo.

## p16_shadowing — valid

El uso interior es bool y el exterior int.

## p17_orden_visibilidad — invalid

Un uso no declarado x [1:6]. Una tabla final no debe validar retroactivamente el uso.

## p18_redeclaracion — invalid

Una redeclaración x [2:5]. Inicializador compatible con bool escrito; se conserva símbolo int original. Sin incompatibilidades extra.

## p19_destino_ausente — invalid

Dos errores: destino ausente [1:0] y + [1:15]. No agregar incompatibilidad de asignación.

## p20_propagacion — invalid

Tres errores: + [1:13], * [2:10] y no_declarada [3:6]. No errores adicionales de inicialización, condición, + exterior ni print.

## p21_hermanos_independientes — invalid

Dos errores: + interior [1:12] y * [1:26]. Sin diagnóstico del + exterior ni print.

## p22_completo_valido — valid

Programa completo; solo mensaje de éxito, no valores impresos por MiniC.

## p23_error_lexico — invalid

Error léxico en @ [1:8]; no ejecutar semántica. Recuperación léxica/sintáctica conforme a prácticas anteriores.

## p24_error_sintactico — invalid

Error sintáctico en ; [1:8]; no ejecutar semántica ni emitir incompatibilidades.
