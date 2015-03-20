#ifndef _NAVE_H_
#define _NAVE_H_
#include "Sprite.h"

class Objeto{
	Sprite * sprite;
	int x;
	int y;
	int w;
	int h;
	bool automovimiento;
	int pasos;
	int pasoActual;
	int pasoLimite;
	int module;//Modulo actual
	bool isVisible;

public:
	void SetVisible(bool isVisible);
	void MoverArribaAbajo(int posicion);
	Objeto(SDL_Surface * screen, char * rutaImagen, int x, int y, int module);//Constructor
	void SetAutoMovimiento(bool automovimiento);
	void SetPasoLimite(int pasos);
	int ObtenerPasoActual();
	void IncrementarPasoActual();
	//bool EstaColicionando(Objeto * b);
	void Pintar();
	void Pintar(int modulo, int x, int y);
	void MoverLados(int posicion);
	void Actualizar();
	int GetY();
	int GetX();
	int GetW();
	int GetH();
	void SetXY(int x, int y);
	void SetX(int x);
	void SetY(int y);

};
#endif