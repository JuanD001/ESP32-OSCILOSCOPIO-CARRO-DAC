#include <Arduino.h>
#include <math.h>

// =====================================================
// PINES DAC
// =====================================================

#define DAC_X 25   // GPIO25 -> CH1 -> X
#define DAC_Y 26   // GPIO26 -> CH2 -> Y


// =====================================================
// CONFIGURACIÓN DEL DIBUJO
// =====================================================

// Centro de la pantalla XY
#define CX 128
#define CY 128

// Velocidad de dibujo 
// (Bajar este número si el dibujo parpadea mucho en la pantalla)
#define DELAY_PUNTO 150


// =====================================================
// FUNCIÓN PARA ENVIAR UN PUNTO AL OSCILOSCOPIO
// =====================================================

void punto(float x, float y)
{
    // Restamos el valor a 255 para invertir las coordenadas
    int X = constrain((int)(255 - x), 0, 255);
    int Y = constrain((int)(255 - y), 0, 255);

    dacWrite(DAC_X, X);
    dacWrite(DAC_Y, Y);

    delayMicroseconds(DELAY_PUNTO);
}


// =====================================================
// DIBUJAR UNA LÍNEA
// =====================================================

void linea(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;

    float distancia = sqrt(dx * dx + dy * dy);

    int pasos = max(2, (int)(distancia / 2));

    for (int i = 0; i <= pasos; i++)
    {
        float t = (float)i / pasos;

        float x = x1 + dx * t;
        float y = y1 + dy * t;

        punto(x, y);
    }
}


// =====================================================
// DIBUJAR CÍRCULO
// =====================================================

void circulo(float cx, float cy, float radio)
{
    const int segmentos = 40;

    for (int i = 0; i <= segmentos; i++)
    {
        float angulo = 2.0 * PI * i / segmentos;

        float x = cx + radio * cos(angulo);
        float y = cy + radio * sin(angulo);

        punto(x, y);
    }
}


// =====================================================
// DIBUJAR RUEDA ESTÁTICA
// =====================================================

void rueda(float cx, float cy, float radio)
{
    // Circunferencia exterior (llanta gruesa)
    circulo(cx, cy, radio);

    // Circunferencia interior (rin vacío para igualar la imagen)
    circulo(cx, cy, radio * 0.4);
}


// =====================================================
// DIBUJAR CARRO (Estilo imagen de referencia)
// =====================================================

void dibujarCarro()
{
    // =================================================
    // CHÁSIS / CARROCERÍA EXTERIOR (Trazo optimizado)
    // =================================================

    // --- Parte Trasera y Techo ---
    linea(40, 170, 35, 155);   // Curva inferior trasera
    linea(35, 155, 45, 130);   // Bumper trasero subiendo
    linea(45, 130, 60, 130);   // Pequeño baúl
    linea(60, 130, 90, 80);    // Caída del vidrio trasero
    linea(90, 80, 145, 80);    // Techo recto

    // --- Parte Delantera ---
    linea(145, 80, 180, 130);  // Parabrisas
    linea(180, 130, 210, 135); // Capó bajando suave
    linea(210, 135, 220, 150); // Frente superior
    linea(220, 150, 215, 170); // Bumper delantero inferior
    linea(215, 170, 200, 170); // Base delantera antes de la rueda

    // --- Espacios Inferiores (dejando huecos para los guardabarros) ---
    linea(150, 170, 105, 170); // Base central (entre ruedas)
    linea(55, 170, 40, 170);   // Base trasera hacia el bumper


    // =================================================
    // VENTANAS (Trazo continuo interno)
    // =================================================

    linea(95, 88, 140, 88);    // Borde superior
    linea(140, 88, 170, 128);  // Borde derecho (parabrisas interno)
    linea(170, 128, 72, 128);  // Línea horizontal de cintura
    linea(72, 128, 95, 88);    // Borde izquierdo (trasero interno)
    
    // Pilar central separador de ventanas
    linea(115, 88, 115, 128);


    // =================================================
    // DETALLES ADICIONALES
    // =================================================

    // Manija de la puerta
    linea(125, 142, 138, 142);

    // Faro delantero (ligeramente sobresaliente)
    linea(215, 145, 225, 145);


    // =================================================
    // RUEDAS
    // =================================================

    // Rueda trasera izquierda
    rueda(80, 170, 25);
    
    // Rueda delantera derecha
    rueda(175, 170, 25);
}


// =====================================================
// CONFIGURACIÓN INICIAL
// =====================================================

void setup()
{
    // Llevar inicialmente el punto al centro
    dacWrite(DAC_X, 128);
    dacWrite(DAC_Y, 128);

    delay(500);
}


// =====================================================
// PROGRAMA PRINCIPAL
// =====================================================

void loop()
{
    // Dibujar continuamente
    dibujarCarro();

    // Pequeña pausa antes del próximo barrido
    delay(5);
}