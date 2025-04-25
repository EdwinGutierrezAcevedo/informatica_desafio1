#ifndef FUNCIONES_H
#define FUNCIONES_H

unsigned char* loadPixels(QString input, int &width, int &height);
bool exportImage(unsigned char* pixelData, int width,int height, QString archivoSalida);
unsigned int* loadSeedMasking(const char* nombreArchivo, int &seed, int &n_pixels);


void rotarCanales(unsigned char* pixelData, int totalBytes, int rotateAmount, bool toLeft);// Operaacion Rotacion
unsigned char rotarDerecha(unsigned char byte, int count);
unsigned char rotarIzquierda(unsigned char byte, int count);
bool compararDato(int dato, int datoB);
bool verficarOperacionXor(unsigned char arrImagen,unsigned chararrMascara,unsigned int arrTxt,unsigned chararrXor, int tamMascara, int semilla );
void xorOperacion(unsigned char* pixelData, unsigned char arrMascara,unsigned intArrTxt, unsigned char* otherData, int totalBytes,int tamMascara,int semilla);//Operacion Xor
#endif // FUNCIONES_H
