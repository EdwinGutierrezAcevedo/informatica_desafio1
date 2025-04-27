/*
 * Programa para reconstruir imagenes BMP en C++ usando Qt.
 *
 * Descripcion:
 * Este programa realiza las siguientes tareas:
 * 1. Carga la imagen modificada, del XOR y de la mascara; los archivos de enmascaramiento
 *    que contienen la semilla y los resultados del aplicados a una versión transformada
 *    de la imagen, en forma de tripletas RGB. Cada uno se guarda en un arreglo dinamico lineal.
 *
 * 2. Verifica con cada mascara y modifica los valores RGB para revertir el proceso que se le aplico.
 *
 * 3. Exporta la imagen reconstruida a un nuevo archivo BMP.
 *
 * 4. Muestra en consola los tipos de operaciones a nivel de bits que se le aplicaron a la imagen original y en que orden.
 *
 * 5. Gestiona la memoria dinámicamente, liberando los recursos utilizados.
 *
 * Entradas:
 * - Cantidad n de mascaras utilizadas o numero de archivos txt.
 * - Archivo de imagen BMP de entrada ("Pn.bmp").
 * - Archivo de salida para guardar la imagen modificada ("I_D.bmp").
 * - Archivos de texto ("Mn.txt") que contienen:
 *     • Una línea con la semilla inicial (offset).
 *     • Varias líneas con tripletas RGB resultantes del enmascaramiento.
 *
 * Salidas:
 * - Imagen BMP modificada ("I_D.bmp").
 * - Operaciones a nivel de bits aplicadas a nivel de bit en el orden que se aplicaron.
 *
 * Requiere:
 * - Librerías Qt para manejo de imágenes (QImage, QString).
 * - No utiliza estructuras ni STL. Solo arreglos dinámicos y memoria básica de C++.
 *
 * Autores: Edwin Gutierrez y Angel Ramirez
 * Fecha: 27/04/2025
 */

#include <fstream>
#include <iostream>
#include <QString>
#include <QCoreApplication>
#include <QImage>
#include "FunciH.h"

using namespace std;

int main()
{
    // Definición de rutas de archivos de entrada (imagen modificada, XOR y mascara) y salida (imagen original)
    int n;
    cout << "Ingrese el numero de enmascaramientos hechos: ";
    cin >> n;
    QString archivoEntrada = "P" + QString::number(n) + ".bmp";
    QString archivoSalida = "I_D.bmp";
    QString archivoXOR = "I_M.bmp";
    QString archivoMascara = "M.bmp";

    // Variables para almacenar las dimensiones de la imagen
    int height = 0;
    int width = 0;

    // Carga la imagen BMP en memoria dinamica y obtiene ancho y alto
    unsigned char *pixelData = loadPixels(archivoEntrada, width, height);
    unsigned char *pixelMascara = loadPixels(archivoMascara, width, height);
    unsigned char *arrXOR = loadPixels(archivoXOR, width, height);
    int tamArrOriginal=height*width*3;
    bool exportI;


    // Llamamos a la funcion que procesa los archivos.
    procesarArchivos(n,pixelData,pixelMascara,arrXOR,tamArrOriginal);
    exportI = exportImage(pixelData, width, height, archivoSalida);
    // Libera la memoria usada para los pixeles
    delete[] pixelData;
    pixelData = nullptr;
    delete[] pixelMascara;
    pixelMascara = nullptr;
    delete[] arrXOR;
    arrXOR = nullptr;

    return 0; // Fin del programa
}


