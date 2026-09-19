int a = 15;
int b = 30;
bool bandera = false;

// Comentario colado para ver que no se lee $ & |
if (a < b && !bandera) {
    while (a <= 20) {
        a = a + 1;
        print(a);
    }
} else {
    bandera = true;
    print(b);
}