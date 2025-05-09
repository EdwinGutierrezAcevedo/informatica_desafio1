#include <fstream>
#include <iostream>
#include <QCoreApplication>
#include <QImage>

using namespace std;

bool compararDato(int dato, int datoB){
    bool ban = true;
    if(dato!=datoB){
        ban = false;
        return ban;
    }else{
        return ban;
    }
}



bool verficarOperacionXor(unsigned char *arrImagen,unsigned char *arrMascara,unsigned int *arrTxt,unsigned char *arrXor, int tamMascara, int semilla ){
    bool ban = true;
    for(int i = 0; i<tamMascara; i++){
        unsigned char data = arrImagen[semilla + i] ^ arrXor[semilla +i];        //inicia desde la semilla dato por dato
        int temporal = static_cast<int>(data)+ static_cast<int>(arrMascara[i]);
        if(compararDato(temporal,(arrTxt[i]))== false) {
            ban = false;                                                          //si un dato no coincide, se sale de la funcion
            return ban;
        }
    }
    return ban;
}
// Operacion Xor
bool xorOperacion(unsigned char* pixelData, unsigned char *arrMascara, unsigned int *ArrTxt, unsigned char* otherData, int totalBytes, int tamMascara, int semilla, int numOrden) {
    bool cambio=true;
    bool res = verficarOperacionXor(pixelData,arrMascara,ArrTxt,otherData, tamMascara, semilla);
    if(res){
        for (int i = 0; i < totalBytes; ++i) {
            pixelData[i] = pixelData[i] ^ otherData[i];                          //se hace la operacion a toda la imagen si se comprueba que
        }
        cout<<numOrden<<")"<<" La operacion bit a bit aplicada fue xor"<<endl;
        return cambio;
    }
    else{
        cambio=false;
        return cambio;
    }
}

//Rotacion

bool verificarOperacionRotacion(unsigned char *arrImagen, unsigned char *arrMascara, unsigned int *arrTxt, int tamMascara, int semilla, int numeroRot, bool izquierda) {

    for(int i = 0; i < tamMascara; i++) {
        // Aplicar rotación circular
        unsigned char data = (izquierda)
                                 ? (arrImagen[semilla + i] << numeroRot) | (arrImagen[semilla + i] >> (8 - numeroRot))
                                 : (arrImagen[semilla + i] >> numeroRot) | (arrImagen[semilla + i] << (8 - numeroRot));

        // Realizar la operación de enmascaramiento
        int temporal = static_cast<int>(data) + static_cast<int>(arrMascara[i]);

        // Comparar con los valores esperados
        if (!compararDato(temporal, arrTxt[i])) {
            return false; // Salimos en la primera falla
        }
    }

    return true;
}


bool rotacionOperacion(unsigned char *pixelData, unsigned char *arrMascara, unsigned int *ArrTxt, int totalBytes, int tamMascara, int semilla, int numeroRot, bool izquierda, int numOrden) {
    bool cambio=true;
    bool res = verificarOperacionRotacion(pixelData, arrMascara, ArrTxt, tamMascara, semilla, numeroRot, izquierda);
    string dir = (izquierda) ? "derecha":"izquierda";
    if (res) {
        for (int i = 0; i < totalBytes; ++i) {
            pixelData[i] = (izquierda)
            ? (pixelData[i] << numeroRot) | (pixelData[i] >> (8 - numeroRot))
            : (pixelData[i] >> numeroRot) | (pixelData[i] << (8 - numeroRot));
        }
        cout<<numOrden<<")"<<" La operacion bit a bit aplicada fue una rotacion de "<< numeroRot << " bits a la "<< dir <<endl;
        return cambio;
    }
    else{
        cambio=false;
        return cambio;
    }
}


//Desplazamiento

// Función para contar ceros al inicio en la representación binaria de 8 bits
int contarCerosInicio(unsigned char num) {
    int ceros = 0;
    for (int i = 7; i >= 0; i--) { // Iterar desde el bit más significativo
        if ((num >> i) & 1) break; // Si encontramos un '1', detenemos el conteo
        ceros++;
    }
    return ceros;
}

// Función para contar ceros al final en la representación binaria de 8 bits
int contarCerosFinal(unsigned char num) {
    int ceros = 0;
    for (int i = 0; i < 8; i++) { // Iterar desde el bit menos significativo
        if ((num >> i) & 1) break; // Si encontramos un '1', detenemos el conteo
        ceros++;
    }
    return ceros;
}

// Función que verifica si todo el arreglo presenta un desplazamiento uniforme
bool verificarDesplazamiento(unsigned char* arr, int tam) {
    if (tam < 2) return false; // Se necesita al menos dos elementos para verificar

    int cerosInicioReferencia = contarCerosInicio(arr[0]);
    int cerosFinalReferencia = contarCerosFinal(arr[0]);

    for (int i = 1; i < tam; i++) {
        int cerosInicio = contarCerosInicio(arr[i]);
        int cerosFinal = contarCerosFinal(arr[i]);

        // Si los ceros iniciales y finales no coinciden con el primero, no hay desplazamiento uniforme
        if (cerosInicio != cerosInicioReferencia || cerosFinal != cerosFinalReferencia) {
            return false;
        }
    }

    return true; // Se encontró un patrón de desplazamiento uniforme
}

bool obtenerDesplazamiento(unsigned char* arr, int tam, int& numeroDesp, bool& izquierda) {
    if (tam < 2) return false;

    int cerosInicioReferencia = contarCerosInicio(arr[0]);
    int cerosFinalReferencia = contarCerosFinal(arr[0]);

    for (int i = 1; i < tam; i++) {
        int cerosInicio = contarCerosInicio(arr[i]);
        int cerosFinal = contarCerosFinal(arr[i]);

        if (cerosInicio != cerosInicioReferencia || cerosFinal != cerosFinalReferencia) {
            return false;
        }
    }

    // Definir dirección y cantidad de bits desplazados
    if (cerosInicioReferencia > 0) {
        numeroDesp = cerosInicioReferencia;
        izquierda = false; // Desplazamiento a la derecha
    } else if (cerosFinalReferencia > 0) {
        numeroDesp = cerosFinalReferencia;
        izquierda = true;  // Desplazamiento a la izquierda
    } else {
        return false;  // No hubo desplazamiento claro
    }

    return true;
}

// Modificar `despOperacion` para recibir desplazamiento detectado
bool despOperacion(unsigned char* pixelData, unsigned char* arrMascara, int totalBytes, int tamMascara, int numOrden) {
    int numeroDesp;
    bool izquierda;

    // Verificar desplazamiento antes de aplicarlo
    if (!obtenerDesplazamiento(arrMascara, tamMascara, numeroDesp, izquierda)) {
        return false;
    }

    // Aplicar desplazamiento correcto
    for (int i = 0; i < totalBytes; ++i) {
        pixelData[i] = (izquierda) ? pixelData[i] << numeroDesp : pixelData[i] >> numeroDesp;
    }

    cout << numOrden << ") La operación bit a bit aplicada fue un desplazamiento de " << numeroDesp << " bits a la " << (izquierda ? "izquierda" : "derecha") << endl;
    return true;
}




unsigned char* loadPixels(QString input, int &width, int &height){
    /*
 * @brief Carga una imagen BMP desde un archivo y extrae los datos de píxeles en formato RGB.
 *
 * Esta función utiliza la clase QImage de Qt para abrir una imagen en formato BMP, convertirla al
 * formato RGB888 (24 bits: 8 bits por canal), y copiar sus datos de píxeles a un arreglo dinámico
 * de tipo unsigned char. El arreglo contendrá los valores de los canales Rojo, Verde y Azul (R, G, B)
 * de cada píxel de la imagen, sin rellenos (padding).
 *
 * @param input Ruta del archivo de imagen BMP a cargar (tipo QString).
 * @param width Parámetro de salida que contendrá el ancho de la imagen cargada (en píxeles).
 * @param height Parámetro de salida que contendrá la altura de la imagen cargada (en píxeles).
 * @return Puntero a un arreglo dinámico que contiene los datos de los píxeles en formato RGB.
 *         Devuelve nullptr si la imagen no pudo cargarse.
 *
 * @note Es responsabilidad del usuario liberar la memoria asignada al arreglo devuelto usando `delete[]`.
 */

    // Cargar la imagen BMP desde el archivo especificado (usando Qt)
    QImage imagen(input);

    // Verifica si la imagen fue cargada correctamente
    if (imagen.isNull()) {
        cout << "Error: No se pudo cargar la imagen BMP." << std::endl;
        return nullptr; // Retorna un puntero nulo si la carga falló
    }

    // Convierte la imagen al formato RGB888 (3 canales de 8 bits sin transparencia)
    imagen = imagen.convertToFormat(QImage::Format_RGB888);

    // Obtiene el ancho y el alto de la imagen cargada
    width = imagen.width();
    height = imagen.height();

    // Calcula el tamaño total de datos (3 bytes por píxel: R, G, B)
    int dataSize = width * height * 3;

    // Reserva memoria dinámica para almacenar los valores RGB de cada píxel
    unsigned char* pixelData = new unsigned char[dataSize];

    // Copia cada línea de píxeles de la imagen Qt a nuestro arreglo lineal
    for (int y = 0; y < height; ++y) {
        const uchar* srcLine = imagen.scanLine(y);              // Línea original de la imagen con posible padding
        unsigned char* dstLine = pixelData + y * width * 3;     // Línea destino en el arreglo lineal sin padding
        memcpy(dstLine, srcLine, width * 3);                    // Copia los píxeles RGB de esa línea (sin padding)
    }

    // Retorna el puntero al arreglo de datos de píxeles cargado en memoria
    return pixelData;
}

bool exportImage(unsigned char* pixelData, int width,int height, QString archivoSalida){
    /*
 * @brief Exporta una imagen en formato BMP a partir de un arreglo de píxeles en formato RGB.
 *
 * Esta función crea una imagen de tipo QImage utilizando los datos contenidos en el arreglo dinámico
 * `pixelData`, que debe representar una imagen en formato RGB888 (3 bytes por píxel, sin padding).
 * A continuación, copia los datos línea por línea a la imagen de salida y guarda el archivo resultante
 * en formato BMP en la ruta especificada.
 *
 * @param pixelData Puntero a un arreglo de bytes que contiene los datos RGB de la imagen a exportar.
 *                  El tamaño debe ser igual a width * height * 3 bytes.
 * @param width Ancho de la imagen en píxeles.
 * @param height Alto de la imagen en píxeles.
 * @param archivoSalida Ruta y nombre del archivo de salida en el que se guardará la imagen BMP (QString).
 *
 * @return true si la imagen se guardó exitosamente; false si ocurrió un error durante el proceso.
 *
 * @note La función no libera la memoria del arreglo pixelData; esta responsabilidad recae en el usuario.
 */

    // Verificar que pixelData no sea nulo
    if (pixelData == nullptr) {
        return false;
    }
    // Crear una nueva imagen de salida con el mismo tamaño que la original
    // usando el formato RGB888 (3 bytes por píxel, sin canal alfa)
    QImage outputImage(width, height, QImage::Format_RGB888);

    // Copiar los datos de píxeles desde el buffer al objeto QImage
    for (int y = 0; y < height; ++y) {
        // outputImage.scanLine(y) devuelve un puntero a la línea y-ésima de píxeles en la imagen
        // pixelData + y * width * 3 apunta al inicio de la línea y-ésima en el buffer (sin padding)
        // width * 3 son los bytes a copiar (3 por píxel)
        memcpy(outputImage.scanLine(y), pixelData + y * width * 3, width * 3);
    }

    // Guardar la imagen en disco como archivo BMP
    if (!outputImage.save(archivoSalida, "BMP")) {
        // Si hubo un error al guardar, mostrar mensaje de error
        cout << "Error: No se pudo guardar la imagen BMP modificada.";
        return false; // Indica que la operación falló
    } else {
        // Si la imagen fue guardada correctamente, mostrar mensaje de éxito
        cout << "Imagen BMP reconstruida fue guardada como " << archivoSalida.toStdString() << endl;
        return true; // Indica éxito
    }
}

unsigned int* loadSeedMasking(const char* nombreArchivo, int &seed, int &n_pixels){
    /*
 * @brief Carga la semilla y los resultados del enmascaramiento desde un archivo de texto.
 *
 * Esta función abre un archivo de texto que contiene una semilla en la primera línea y,
 * a continuación, una lista de valores RGB resultantes del proceso de enmascaramiento.
 * Primero cuenta cuántos tripletes de píxeles hay, luego reserva memoria dinámica
 * y finalmente carga los valores en un arreglo de enteros.
 *
 * @param nombreArchivo Ruta del archivo de texto que contiene la semilla y los valores RGB.
 * @param seed Variable de referencia donde se almacenará el valor entero de la semilla.
 * @param n_pixels Variable de referencia donde se almacenará la cantidad de píxeles leídos
 *                 (equivalente al número de líneas después de la semilla).
 *
 * @return Puntero a un arreglo dinámico de enteros que contiene los valores RGB
 *         en orden secuencial (R, G, B, R, G, B, ...). Devuelve nullptr si ocurre un error al abrir el archivo.
 *
 * @note Es responsabilidad del usuario liberar la memoria reservada con delete[].
 */

    // Abrir el archivo que contiene la semilla y los valores RGB
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        // Verificar si el archivo pudo abrirse correctamente
        cout << "No se pudo abrir el archivo." << endl;
        return nullptr;
    }

    // Leer la semilla desde la primera linea del archivo
    archivo >> seed;

    int r, g, b;

    // Contar cuantos grupos de valores RGB hay en el archivo
    // Se asume que cada linea después de la semilla tiene tres valores (r, g, b)
    while (archivo >> r >> g >> b) {
        n_pixels++;  // Contamos la cantidad de píxeles
    }

    // Cerrar el archivo para volver a abrirlo desde el inicio
    archivo.close();
    archivo.open(nombreArchivo);

    // Verificar que se pudo reabrir el archivo correctamente
    if (!archivo.is_open()) {
        cout << "Error al reabrir el archivo." << endl;
        return nullptr;
    }

    // Reservar memoria dinamica para guardar todos los valores RGB
    // Cada píxel tiene 3 componentes: R, G y B
    unsigned int* RGB = new unsigned int[n_pixels * 3];

    // Leer nuevamente la semilla desde el archivo (se descarta su valor porque ya se cargó antes)
    archivo >> seed;

    // Leer y almacenar los valores RGB uno por uno en el arreglo dinámico
    for (int i = 0; i < n_pixels * 3; i += 3) {
        archivo >> r >> g >> b;
        RGB[i] = r;
        RGB[i + 1] = g;
        RGB[i + 2] = b;
    }

    // Cerrar el archivo después de terminar la lectura
    archivo.close();

    // Retornar el puntero al arreglo con los datos RGB
    return RGB;
}



void unionOperacion(unsigned char *pixelData, unsigned char *arrMascara, unsigned int *ArrTxt, unsigned char *arrXor, int totalBytes, int tamMascara, int semilla, int numOrden) {
    for (int i = 1; i <= 3; i++) {
        bool cambio = false;
        switch (i) {
        case 1:
            // Operacion XOR
            cambio = xorOperacion(pixelData, arrMascara, ArrTxt, arrXor, totalBytes, tamMascara, semilla, numOrden); //verdadero si se hizo XOR, falso de otro modo
            if(cambio)break; // se sale si es verdadero
        case 2:
            int numeroDesp;
            cambio = despOperacion(pixelData,arrMascara,totalBytes,tamMascara,numOrden);
            if (cambio) break;
        case 3:
            for (int k = 1; k < 8; k++) { // Rotacion
                bool izquierda = true;    // direccion
                cambio = rotacionOperacion(pixelData, arrMascara, ArrTxt, totalBytes, tamMascara, semilla, k, izquierda, numOrden);
                if (cambio) break;
                izquierda = !izquierda;  //no izquierda, osea, derecha
                cambio = rotacionOperacion(pixelData, arrMascara, ArrTxt, totalBytes, tamMascara, semilla, k, izquierda, numOrden);
                if (cambio) break;
            }
        }
        break;
    }
}

void procesarArchivos(int numArchivos, unsigned char *pixelData, unsigned char *arrMascara, unsigned char *arrXor, int totalBytes) {
    for (int j = numArchivos; j >= 0; --j) {
        // Construcción del nombre del archivo usando concatenacion
        QString nombreArchivo = "M" + QString::number(j) + ".txt";

        int seed = 0;
        int n_pixels = 0;

        // Llamar a la funcion de carga
        unsigned int* maskingData = loadSeedMasking(nombreArchivo.toStdString().c_str(), seed, n_pixels); //cargamos el Mj.txt

        if (maskingData != nullptr) {
            int tamMascara = n_pixels * 3;   //tamaño de la mascara
            unionOperacion(pixelData, arrMascara, maskingData, arrXor, totalBytes, tamMascara, seed, j+1);  //rectifica y hace operaciones

            // Liberar la memoria reservada.
            delete[] maskingData;
        } else {
            cout << "Error al cargar el archivo: " << nombreArchivo.toStdString() <<endl;
            break;
        }

        cout << "----------------------------------------" <<endl;
    }
}







