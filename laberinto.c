// Estas son directivas del operador  # cualquier linea q empiece con # no es una instruccion q la computadora ejecuta
// son ordenes para el preprocesador , un programa q revisa el codigo antes q se convierta en una aplicacion 
//include eeta palabra le dice al compilador Busca este archivo y pega todo su contenido aqui mismo 

#include <stdio.h>// standrad input output es el q permite usar printf y scanf sin este  no se muestra nada en pantalla
#include <stdlib.h>// standard library contiene funciones como malloc para memoria del laberinto , fre y rand para los numeros aleatorios 
#include <time.h> // es el q permite usar time(NULL) se usa para q el laberinto sea distinto cada ez q lo jugamos usando la hora actual como semilla 

// define el buscador y remplazador , eso crea Macros o constantes  no son variables son simplemente etiquetas 
// le estamos diciendo al procesador  cada vez q encuentres PARED etc cambiala por el simbolo q le asignamos 
//Lo uso para q el codigo sea mas legible define es una instruccion para el procesador no es de c 
#define PARED '#'
#define CAMINO ' '
#define RATON 'R'
#define SALIDA 'S'
#define RASTRO '.'
//definimos nuestras variables de entorno para nuestro mapa /matriz  
//malloc significa Memory Allocation (Asignacion de Memoria ) es la funcion q le pide a la computadora reservame un pedazo de memoria RAM de este tamaño porque lo voy a usar ahora 

int filas, columnas;//declaramos q fila y columnas van a ser numeros enteros positivos 

//char indica q lo q vamos a guardar son caracteres letras etc , 
// el primer asterisco significa q el laberinto no es un caracterer sino un puntero y el segundo le dice q es un puntero a un puntero 
// el puntero guarda la direccion de  la matriz le dice donde comienza y si queremos q sea mas especifica no es dice la direccioon de cualquier elemento 
// el malocc es el q crea el espacio en la memoria para guardar dentro de esos espacios nuestra matriz 

char **laberinto;

// Función para limpiar la memoria RAM antes de cerrar el programa
void liberar_memoria() {
    
    // 1. Liberamos cada fila individualmente
    // ¿QUÉ HACE?: Recorre el arreglo de punteros y libera el bloque de caracteres (columnas).
    // ¿POR QUÉ?: Porque cada fila se creó con un malloc() independiente. 
    // Si no las borramos una por una, esos datos se quedan "huérfanos" en la RAM.
    for (int i = 0; i 
        < filas; i++) {
        free(laberinto[i]); 
    }

    // 2. Liberamos el puntero maestro
    // ¿QUÉ HACE?: Libera el bloque de memoria que guardaba las direcciones de las filas.
    // ¿POR QUÉ?: Al principio pedimos memoria para una "lista de flechas" (char**). 
    // Este free elimina esa lista para que la limpieza sea del 100%.
    free(laberinto);
}

// Función para imprimir el laberinto en pantalla
void imprimir_laberinto() {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            printf("%c ", laberinto[i][j]);
        }
        printf("\n");
    }
}

// Algoritmo de generación: Excavación por profundidad (DFS)
void generar_laberinto(int f, int c) {
    int dir[] = {0, 1, 2, 3}; // 0:Arriba, 1:Abajo, 2:Izquierda, 3:Derecha
    for (int i = 0; i < 4; i++) { // Mezclar direcciones aleatoriamente
        int r = rand() % 4; // rand() es un metodo q me genera un numero gigante aleatorio al dividirlo con 4  su mod siempre me dera 0.1.2.3
        int temp = dir[r];// temp significa temporal , metemos el valor de dir en la posicion r q generamos dentro de temp
        dir[r] = dir[i];//dir[r] = dir[i], el valor de dir[r] se sobreescribe (se pierde de esa posición), pero como ya lo guardamos "puesto a salvo" en temp, no pasa nada
        dir[i] = temp;// al final cambiamos el valor de dir en la posicon iterada por el valor temporal q habiamos guardado 
    }// hacemos esto para q dir siempre tenga un orden diferente 

    for (int i = 0; i < 4; i++) {
        int nf = f, nc = c;// nf significa nueva fila , nc nueva columna 
        if (dir[i] == 0) nf -= 2;// esto es un operador de asignacion compuesta le decimos q si dir es igual a cero entonces restamos de nf -2 Arriba: Restas 2 a la fila (subes en la matriz)
        else if (dir[i] == 1) nf += 2; // Abajo: Sumas 2 a la fila (bajas en la matriz)
        else if (dir[i] == 2) nc -= 2; // Izquierda: Restas 2 a la columna (vas hacia atrás)
        else if (dir[i] == 3) nc += 2; // Derecha: Sumas 2 a la columna (vas hacia adelante)

        /* QUE PREGUNTASMI EN ESTE IF ?
        
            nf > 0: La nueva fila está por debajo del borde superior
            nf < filas - 1: La nueva fila está por encima del borde inferior

            nc > 0: La nueva columna está a la derecha del borde izquierdo

            nc < columnas - 1: la nueva columna está a la izquierda del borde derecho

            laberinto[nf][nc] == PARED: Ese lugar al que quiero saltar es pared 

        
        */
        /*recordar q nuestra matriz ya esta llena de muros*/
        /*usuamos la recursividad aca porque si el dir q tenemos cumple y es impar y no choca con los bordes 
        entonces es igual a camino q es espacio vacio , usamos la recursivad despues para ir mas profundo dentro de la matriz 
        asi cuando la matriz ya este todos los caminos hechos la recursividad para y los hijos le devuelven al padre la matriz ya con 
        todos los caminos de nuestro laberinto , C no retorna el laberinto sino el turno por asi decirlo por cada vez q usamos la recurcion
        creamos un for nuevo cada q llamamos a generar laberinto por eso cuando ya el laberinto termine y estee en el ultimo for
        C lo retorna automaticamente cuando se itere todo ese for , lo regresa al for anterior y este ve q el laberinto ya esta listo 
        asi q lo retorna cuando se itera todo tambien y retorna el turno asi sucecivamenete  hasta el for padre y este hace lo mismo y sale de la funcion
        
        se retorna automaticamente me refiero a q el for termina de iterar llega  a la llave final y dvuelve el turno al for anterior q lo llamo 

        turno me refiero a q cada for pinta en laberinto un camino pero usando la recursividad  entonces el turno del for q estaba pintando 
        se queda congelado porq llamo a otro for para pintar la sgt parte y asi sucecivamente 
        
        esto funciona porque nosotros si nuestro minero por asi decirlo el punto donde escabamos es impar 
        la matematica nos dice q cada numero impar sumado + 2  nosda un numero impar por eso todos los lugares  q escabamos son impares
        y si no son en ciertos casos entonces son puentes y si la casilla de alado es impar la q esta alado del puente q es par e impar
        entonces el puente sera un camino sino sera una pared 
        
        como funciona los puentes  ,los puentes siempre tiene un par y un impar en fila y columna entonces
        usamos la formula del punto medio si tenemos dos puntos en una recta x1 y x2 el punto q esta justo en el centro se calcula 
        como promedio , en este caso nos daria la distancia o una posicion en la matriz q esta en medio de los dos objetos o posiciones 
        Porq siempre el puente es par,impar o viceverza ?

        Porq al comenzar excabando desde un punto impar 1,1  al sumarle 2 pasos siempre por regla general siempre cae en una casilla impar
        y  si nosotros dimos dos pasos hacia delante y nuestro puente es el del medio  -1 de nuestra posicion inicial siempre nuestro 
        puente sera par , impar (2,1) por una propiedad fisica de las matrices q se vuelve una ley matematica cuando decidimos saltar de 2 en 2 


        */

        if (nf > 0 && nf < filas - 1 && nc > 0 && nc < columnas - 1 && laberinto[nf][nc] == PARED) {
            laberinto[nf][nc] = CAMINO;// le decimos q laberinto  va ser igual a camino en la posicion de nueva fila y nueva columna 
            laberinto[f + (nf - f) / 2][c + (nc - c) / 2] = CAMINO;//El puente ejemplo (la fórmula): laberinto[3 + (3-3)/2][3 + (1-3)/2] laberinto[3][2]
            generar_laberinto(nf, nc);
        }
    }
}

// Algoritmo de resolución: Backtracking
int resolver(int f, int c) {
 
    if (laberinto[f][c] == SALIDA) return 1;
    // Marcamos como visitado con un punto
    
    if (laberinto[f][c] == CAMINO) laberinto[f][c] = RASTRO;
/*(Arreglo/Matriz unidimensional): Los corchetes le dicen a C que no es una variable común que guarda
 un solo número (como int x = 5;), sino que es una lista o colección de números guardados uno al lado del otro en la memoria.
 
 df y dc significan direccion fila , direccin columna , esto atravez del indice va cambiando los valores
    -1,0 Arriba 0
    1,0 Abajo 1
    0,-1 Izquierda 2
    0,1 Derecha 3
 */

    int df[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; i++) {
        int nf = f + df[i];// segun la iteracion nueva fila y nueva columana tomaran el valor de direccion 
        int nc = c + dc[i];
/* esta es la condicion q nos verifica q nuestras posiciones no salgan de los rangos los bordes*/
        if (nf >= 0 && nf < filas && nc >= 0 && nc < columnas && 
           (laberinto[nf][nc] == CAMINO || laberinto[nf][nc] == SALIDA)) {
            // en lenguaje se no exisiten variables boleanas como tales  se usa 0 y un valor mayor a cero en este caso 1 
            /* para saber si es falso 0  si es verdadero 1 
            entonces usamos la recursividad para hallar la salida en cada posicion entra en nuestro caso base 
            y si no entra en nuestro caso base entra en una casilla camino q despues se convierte en nuestro rastro 
            y asi vamos recorriendo atravez de la recursividad el la berinto pasandole al funcion las posiciones de 
            nueva fila y nueva columna en cada recursion y si entra en nuestro caso base nos retornara 1  para salir de nuestro bucle infinito 
     
            
    -1,0 Arriba 0
    1,0 Abajo 1
    0,-1 Izquierda 2
    0,1 Derecha 3

    
            */
            if (resolver(nf, nc)) return 1;
        }
    }
    /* 
*/


    // Si no es el camino correcto, quitamos el rastro (Backtrack)
    /* lo que pasa cuando usamos el backtrack, es q llegado a un lugar donde solo hay muros 
    o un camino donde hay Rastros , el bucle for termina y no se movio el minero por asi decilor o el raton
     entonces entra en esta condicion  le dice q si laberinto en tal posicin es igual a rastro osea la posicion en la 
     que estamos actualmente es igual a rastro la convierte le asigna el valor de camnio y asi sucesivamente si es q 
     nos encontramos en un callejon sin salida casilla por casilla hacia tras efecto domino  , depende tambien eso de nuestra movilidad 
     de como nos movemos el como sale de las ciutacios  o entra en ellas       
    -1,0 Arriba 0
    1,0 Abajo 1
    0,-1 Izquierda 2
    0,1 Derecha 3   
    


    
    
    
    */
    if (laberinto[f][c] == RASTRO) laberinto[f][c] = CAMINO;
    return 0;
}

int main(int argc, char *argv[]) {
    // 1. Configurar tamaño (por defecto o por argumentos)
    /* EXPLICACIÓN DE LOS PARÁMETROS:
   argc: (Argument Count) Contador de palabras ingresadas en la terminal.
 
   argv: (Argument Vector) Lista de las palabras ingresadas. argv[0] es el nombre del programa.
   atoi: (Ascii TO Integer) Función que convierte texto (letras) a números enteros.
   ?:    Operador ternario. Es un "if" en miniatura. (condicion ? verdadero : falso)
*/

// PREGUNTA: ¿El usuario escribió un número de filas en la consola?
// Si argc > 1 es verdad, usa ese número convertido por atoi. Si no, usa 11 por defecto.
    filas = (argc > 1) ? atoi(argv[1]) : 11;
// aca hacemos lo mismo 
    columnas = (argc > 2) ? atoi(argv[2]) : 11;
    // Asegurar que sean impares para que el generador funcione bien
   // Verificamos si las filas son pares usando el operador residuo (%)
// Si el resto de dividir por 2 es 0, significa que es par.
    if (filas % 2 == 0) filas++; // Si es par, le sumamos 1 para volverlo impar.

// Hacemos lo mismo con las columnas.
    if (columnas % 2 == 0) columnas++; // Garantizamos que siempre sea impar.

// Inicializamos la semilla para los números aleatorios.
// Usamos time(NULL) para que el laberinto sea distinto cada vez que ejecutas el programa.
/**/
    srand(time(NULL));




// 1. Reservamos memoria para la "columna vertebral" del laberinto (el índice de filas).
// Pedimos espacio para guardar 'filas' cantidad de punteros (char*).
// Usamos sizeof(char*) porque estamos guardando DIRECCIONES (flechas), no letras todavía.


// 2. Entramos a construir cada fila una por una.
    laberinto = (char **)malloc(filas * sizeof(char *));
    for (int i = 0; i < filas; i++) {
        // Reservamos el espacio real para los caracteres de esta fila (las columnas).
    // Aquí usamos sizeof(char) porque ahora sí vamos a guardar DATOS (ladrillos como '#' o ' ').
        laberinto[i] = (char *)malloc(columnas * sizeof(char));
        // 3. Una vez que la fila tiene memoria, la llenamos de punta a punta
        // Inicializamos todo como un bloque sólido de PARED (#).
        for (int j = 0; j < columnas; j++) laberinto[i][j] = PARED;
    }

    // 3. Generar estructura
    laberinto[1][1] = CAMINO;
    generar_laberinto(1, 1);

    // 4. Colocar Personaje (Raton) y Salida (S) entre muros
    laberinto[1][1] = RATON;
    // Colocamos la S en la esquina inferior, asegurando que esté entre muros
    laberinto[filas - 2][columnas - 2] = SALIDA;

    printf("\nLaberinto generado para el Raton:\n");
    imprimir_laberinto();

    printf("\nBuscando solucion...\n"); // Sin tilde para evitar errores
    if (resolver(1, 1)) {
        imprimir_laberinto();
        printf("\nEl Raton encontro el queso!\n");
    } else {
        printf("\nNo hay escapatoria para el Raton.\n");
    }

    liberar_memoria();
    return 0;
}