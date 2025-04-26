#ifndef FUNCIONES_H
#define FUNCIONES_H

unsigned char* loadPixels(QString input, int &width, int &height);
bool exportImage(unsigned char* pixelData, int width,int height, QString archivoSalida);
unsigned int* loadSeedMasking(const char* nombreArchivo, int &seed, int &n_pixels);


void rotarCanales(unsigned char* pixelData, int totalBytes, int rotateAmount, bool toLeft);// Operaacion Rotacion
unsigned char rotarDerecha(unsigned char byte, int count);
unsigned char rotarIzquierda(unsigned char byte, int count);
bool compararDato(int dato, int datoB);
bool verficarOperacionXor(unsigned char* arrImagen,unsigned char* arrMascara,unsigned int* arrTxt,unsigned char* arrXor, int tamMascara, int semilla );
bool xorOperacion(unsigned char* pixelData, unsigned char* arrMascara,unsigned int* ArrTxt, unsigned char* otherData, int totalBytes,int tamMascara,int semilla);//Operacion Xor
bool verificarOperacionRotacion(unsigned char* arrImagen, unsigned char* arrMascara, unsigned int* arrTxt, int tamMascara, int semilla, int numeroRot, bool izquierda);
bool rotacionOperacion(unsigned char* pixelData, unsigned char* arrMascara, unsigned int* ArrTxt, int totalBytes, int tamMascara, int semilla, int numeroRot, bool izquierda);
bool verficarOperacionDesplazamiento(unsigned char* arrImagen,unsigned char* arrMascara,unsigned int* arrTxt, int tamMascara, int semilla, int numeroDesp ,bool izquierda );
bool despOperacion(unsigned char* pixelData,unsigned char* arrMascara,unsigned int* ArrTxt, int totalBytes,int tamMascara, int semilla, int numeroDesp ,bool izquierda );
void unionOperacion(unsigned char* pixelData, unsigned char* arrMascara, unsigned int* ArrTxt, unsigned char* arrXor, int totalBytes, int tamMascara, int semilla);
void procesarArchivos(int numArchivos,unsigned char *pixelData, unsigned char *arrMascara, unsigned char *arrXor, int totalBytes);
#endif // FUNCIONES_H



