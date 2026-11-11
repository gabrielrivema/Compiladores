int contador = 3;
bool activo = true;
while (contador > 0 && activo) {
    print(contador);
    contador = contador - 1;
    if (contador == 0) { activo = false; }
}
