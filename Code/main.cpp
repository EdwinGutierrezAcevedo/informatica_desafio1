/*
 * Programa demostrativo de manipulaciónprocesamiento de imágenes BMP en C++ usando Qt.
 *
 * Descripción:
 * Este programa realiza las siguientes tareas:
 * 1. Carga una imagen BMP desde un archivo (formato RGB sin usar estructuras ni STL).
 * 2. Modifica los valores RGB de los píxeles asignando un degradado artificial basado en su posición.
 * 3. Exporta la imagen modificada a un nuevo archivo BMP.
 * 4. Carga un archivo de texto que contiene una semilla (offset) y los resultados del enmascaramiento
 *    aplicados a una versión transformada de la imagen, en forma de tripletas RGB.
 * 5. Muestra en consola los valores cargados desde el archivo de enmascaramiento.
 * 6. Gestiona la memoria dinámicamente, liberando los recursos utilizados.
 *
 * Entradas:
 * - Archivo de imagen BMP de entrada ("I_O.bmp").
 * - Archivo de salida para guardar la imagen modificada ("I_D.bmp").
 * - Archivo de texto ("M1.txt") que contiene:
 *     • Una línea con la semilla inicial (offset).
 *     • Varias líneas con tripletas RGB resultantes del enmascaramiento.
 *
 * Salidas:
 * - Imagen BMP modificada ("I_D.bmp").
 * - Datos RGB leídos desde el archivo de enmascaramiento impresos por consola.
 *
 * Requiere:
 * - Librerías Qt para manejo de imágenes (QImage, QString).
 * - No utiliza estructuras ni STL. Solo arreglos dinámicos y memoria básica de C++.
 *
 * Autores: Augusto Salazar Y Aníbal Guerra
 * Fecha: 06/04/2025
 * Asistencia de ChatGPT para mejorar la forma y presentación del código fuente
 */

#include <fstream>
#include <iostream>
#include <QCoreApplication>
#include <QImage>
#include "FunciH.h"

using namespace std;

int main()
{
    //Def
    // Definición de rutas de archivo de entrada (imagen original) y salida (imagen modificada)
    QString archivoEntrada = "I_O.bmp";
    QString archivoSalida = "I_D.bmp";
    QString archivoXOR = "I_M.bmp";
    QString archivoMascara = "M.bmp";

    // Variables para almacenar las dimensiones de la imagen
    int height = 0;
    int width = 0;

    // Carga la imagen BMP en memoria dinámica y obtiene ancho y alto
    unsigned char *pixelData = loadPixels(archivoEntrada, width, height);
    unsigned char *pixelMascara = loadPixels(archivoMascara, width, height);
    unsigned char *arrXOR = loadPixels(archivoXOR, width, height);
    int tamArrOriginal=height*width*3;
    bool result;



    // xorOperacion(pixelData,arrXOR,tamArrOriginal);

    // Exporta la imagen modificada a un nuevo archivo BMP
    //bool exportI = exportImage(pixelData, width, height, archivoSalida);

    // Muestra si la exportación fue exitosa (true o false)
    //cout << exportI << endl;
    //a


    // Variables para almacenar la semilla y el número de píxeles leídos del archivo de enmascaramiento
    int seed = 0;
    int n_pixels = 0;

    // Carga los datos de enmascaramiento desde un archivo .txt (semilla + valores RGB)
    unsigned int *maskingData = loadSeedMasking("M1.txt", seed, n_pixels);
    int tamArrMascara= n_pixels*3;   //tamano total de la mascara
    //result = verficarOperacionXor(pixelData, pixelMascara, maskingData, arrXOR, tamArrMascara, seed );
    //result = verficarOperacionDesplazamiento(pixelData, pixelMascara, maskingData, tamArrMascara, seed, 2,true );
    //cout<< result<<endl;

    /*
    //enmascaramiento
    for (int i = 0; i < tamArrMascara; i+=3) {                    //S(k)=I_D(k+s) + M(k)
        cout << "Pixel " << i / 3 << ": ("
             <<   static_cast<int>(pixelData[seed + (i)]) + static_cast<int>(pixelMascara[i]) << ", "
             <<  static_cast<int>(pixelData[seed + (i+1)])+ static_cast<int>(pixelMascara[i+1]) << ", "
             <<   static_cast<int>(pixelData[seed + (i+2)]) + static_cast<int>(pixelMascara[i+2]) << ")" << endl;
    }

    cout <<"*************************"<<endl;
    */


     //NO es   lugar
    unionOperacion(pixelData,pixelMascara,maskingData,arrXOR,tamArrOriginal,tamArrMascara,seed);
    bool exportI = exportImage(pixelData, width, height, archivoSalida);
    //paso 2
    unsigned int *maskingData2 = loadSeedMasking("M2.txt", seed, n_pixels);
    unionOperacion(pixelData,pixelMascara,maskingData2,arrXOR,tamArrOriginal,tamArrMascara,seed);
    exportI = exportImage(pixelData, width, height, archivoSalida);
    unsigned int *maskingData3 = loadSeedMasking("M2.txt", seed, n_pixels);
    unionOperacion(pixelData,pixelMascara,maskingData2,arrXOR,tamArrOriginal,tamArrMascara,seed);
    // Libera la memoria usada para los píxeles
    delete[] pixelData;
    pixelData = nullptr;
    // Libera la memoria usada para los datos de enmascaramiento
    if (maskingData != nullptr){
        delete[] maskingData;
        maskingData = nullptr;
    }


    return 0; // Fin del programa
}









