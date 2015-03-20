#ifndef __NAVE_H__
#define __NAVE_H__
#include "Objeto.h"
#include "Config.h"
class Nave
{
	Objeto * nave;
	Objeto * bala[MAXIMO_DE_BALAS];
	int balaVisible;
	bool visible;
	bool colision;
	int tipoNave;
public:
	enum TipoColision{
		NAVE,
		BALA,
	};

	Nave(SDL_Surface*screen, char*rutaImagen, int x, int y, int module, int tipoNave);
	void Pintar();
	void Disparar(int balas);
	void AutoDisparar(int balas);
	void MoverArriba(int velocidad);
	void MoverAbajo(int velocidad);
	void MoverIzquierda(int velocidad);
	void MoverDerecha(int velocidad);
	Objeto* GetNaveObjeto();
	void setVisible(bool visible);
	bool Colision(Nave * nave, TipoColision tipoColision);
	void crearNuevo(int pos);
};
#endif