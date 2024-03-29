/*
Elizabeth Matamoros Bojorge C04652
Kevin Jimenez Acuna C13876
Elian Jimenez Quesada C13983

Programa que recibe dos funciones cuadraticas en donde el usuario ingresa
los valores de los parametros, luego se le solicita al usuario un porcentaje
de error entre 0 y 100, el programa calcula la diferencia entre ambas
funciones, despues se calcula un subdominio donde la diferencia entre ambas
funciones no supere el porcentaje de error elegido por el usuario. Ademas
se grafican las dos funciones ingresadas en donde se indica el subdominio
encontrado.
*/

#include <stdio.h> // biblioteca para funciones de entrada y salida estandar
#include <math.h> // biblioteca para funciones matematicas y operaciones comunes
#include <stdbool.h> // biblioteca para tipos de datos: bool, true y false


/*
funcion que recibe dos parametros: un puntero a una cadena de caracteres (o sea
la alerta) y un puntero a un valor flotante (valor donde se almacena el valor
ingresado por el usuario)
*/
bool solicitarFlotante(const char *alerta, float *valor) {
    printf("%s", alerta); // imprime la alerta, para pedirle al usuario que ingrese el float
    if (scanf("%f", valor) == 1) { // lee el valor flotante por medio del scanf
        return true; // si es exitoso retorna true
    } else { // de lo contrario limpia el buffer de entrada y devuelve false
            // esto con el fin de que al llamar de nuevo a la funcion todo este vacio
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {} 
        return false; 
    }
}


/*
funcion que calcula el porcentaje de error
entre dos valores flotantes
*/
double porcentajeError(double f_x, double g_x) {
    if (f_x == 0) return 0; // si f_x es 0 devuelve 0
    return fabs((f_x - g_x) / f_x) * 100; // sino, se calcula el porcentaje de error
    // se devuelve el porcentaje de error calculado
}


/*
funcion que busca el subdominio entre dos funciones cuadraticas dentro de un
porcentaje de error, recorre un rango de valores de x y calculalos valores
de x para cada funcion y verifica que el porcentaje de error entre ellas
sea menor o igual al porcentaje maximo permitido
*/
bool encontrarSubdominio(double a, double b, double c, double d, double e, double f,
                         double porcentaje_max, double inicio, double fin, double paso,
                         double *subdominio_inicio, double *subdominio_fin) {
    *subdominio_inicio = 0; // inicia los valores apuntando a cero (cambia si se encuentra un subdominio)
    *subdominio_fin = 0; // inicia los valores apuntando a cero (cambia si se encuentra un subdominio)
    bool subdominioEncontrado = false; // esta variable se usara para indicar si se encontro un subdominio valido
    double x = inicio; // punto inicial del rango de valores de x 

    while (x <= fin) { // con el ciclo while se itera sobre un rango dado de valores para x 
        double f_x = a * x * x + b * x + c; // calcula los valores para la funciones con el valor actual de x
        double g_x = d * x * x + e * x + f;

        /*
        se verifica si el porcentaje de error encontrado entre las dos funciones 
        es menor o igual al porcentaje de error maximo permitido (o sea el del usuario)
        */
        if (porcentajeError(f_x, g_x) <= porcentaje_max) {
            if (!subdominioEncontrado) {
                *subdominio_inicio = x;
                subdominioEncontrado = true;
            }
            *subdominio_fin = x;
        } else if (subdominioEncontrado) { // si ya se encontro un subdominio que cumpla se termina el bucle y sale de la funcion
            break;
        }
        x += paso; // incrementa el valor de x por paso
    }

    return subdominioEncontrado;
}


/*
funcion que grafica las funciones cuadraticas generadas a partir de las constantes elegidas por el usuario,
en estas se indica el subdomio encontrado de valores de x para los cuales el porcentaje de error calculado
es menor que el porcentaje de error proporcionado por el usuario
*/
void graficador(double a, double b, double c, double d, double e, double f, double inicio, double fin){
    // se utiliza la biblioteca de graficacion gnuplot
    FILE *fp = popen("gnuplot -persist", "w");

        // valores para el dominio x, con 10 valores adelante y 10 valores atras
        fprintf(fp, "set xrange [%f:%f]\n", inicio - 10, fin + 10);

        // se marca el subdominio encontrado, con lineas - de color negro y grosor 2
        fprintf(fp, "set arrow from %f, graph 0 to %f, graph 1 nohead lc rgb 'black' lw 2 dt '-' \n", inicio, inicio);
        fprintf(fp, "set arrow from %f, graph 0 to %f, graph 1 nohead lc rgb 'black' lw 2 dt '-' \n", fin, fin);

        // escribe el primer valor del subdominio
        fprintf(fp, "set label 1 \"%.2f\" at %f, graph 0.1 right font '1'\n", inicio, inicio);

        // escribe el ultimo valor del subdominio
        fprintf(fp, "set label 2 \"%.2f\" at %f, graph 0.1 left font '1'\n", fin, fin);

        // se escriben las dos dos funciones cuadráticas
        fprintf(fp, "f(x) = %f*x**2 +%f*x +%f \n", a, b, c);
        fprintf(fp, "g(x) = %f*x**2 +%f*x +%f \n", d, e, f);

        // se trazan las graficas de las dos funciones cuadraticas, con su respectivo titulo
        fprintf(fp, "plot f(x) with lines lw 2 lc rgb 'blue' title 'f(x) = %.2fx^2 + %.2fx + %.2f', "
                "g(x) with lines lw 2 lc rgb 'red' title 'g(x) = %.2fx^2 + %.2fx + %.2f'\n", a, b, c, d, e, f);
        fclose(fp);
}


/*bloque principal: se declaran las variables para los coeficientes de las funciones 
cuadraticas, el porcentaje maximo de error y los limites del subdominio*/
int main() {
    float a, b, c, d, e, f, porcentaje_max;
    double inicio, fin;

    // se le solicita al usuario que ingrese los valores necesario para que se ejecute el programa 
    if (!solicitarFlotante("Ingrese un valor flotante para a por favor: ", &a) ||
        !solicitarFlotante("Ingrese un valor flotante para b por favor: ", &b) ||
        !solicitarFlotante("Ingrese un valor flotante para c por favor: ", &c) ||
        !solicitarFlotante("Ingrese un valor flotante para d por favor: ", &d) ||
        !solicitarFlotante("Ingrese un valor flotante para e por favor: ", &e) ||
        !solicitarFlotante("Ingrese un valor flotante para f por favor: ", &f) ||
        !solicitarFlotante("Ingrese el valor para el porcentaje de error entre (0-100): ", &porcentaje_max)) {
        printf("Error al ingresar los datos. Asegurese de ingresar numeros flotantes.\n");
        return 1; 
    }

    // se verifica que el porcentaje ingresado por el usuario este en el rango permitido
    if (porcentaje_max < 0 || porcentaje_max > 100) {
        printf("El porcentaje de error debe estar entre 0 y 100.\n");
        return 1; 
    }

    // se llama a la funcion encontrarSubdominio para buscar un subdominio aceptable
    // se especifica que el rango de x es [-100000,100000] con incremtos de 0.1
    bool encontrado = encontrarSubdominio(a, b, c, d, e, f, porcentaje_max, -100000, 100000, 0.1, &inicio, &fin);

    // si se encuentra un subdominio aceptable se imprime y se grafican las funciones, indicando el subdominio encontrado.
    if (encontrado) {
        printf("Subdominio aceptable: [%.2f, %.2f]\n", inicio, fin);
        graficador(a, b, c, d, e, f, inicio, fin);

    // sino, se imprime este mensaje
    } else {
        printf("No se encontro un subdominio que cumpla con el criterio especificado.\n"); 
    }

    return 0;
}
