#ifndef __NAVE_H__
#define __NAVE_H__
#include "Sprite.h"
#include "Config.h"
#include "SDL_opengl.h"

class Nave
{
	Sprite * nave;
	Sprite * bala[MAXIMO_DE_BALAS];
	int balaVisible;
	bool visible;
	bool colision;
	int tipoNave;
public:
	enum TipoColision{
		NAVE,
		BALA,
	};

	Nave(OpenGlImplement* openGlImplement, char*rutaImagen, int x, int y, int tipoNave);
	void Draw();
	void Disparar(int balas);
	void AutoDisparar(int balas);
	void MoverArriba(int velocidad);
	void MoverAbajo(int velocidad);
	void MoverIzquierda(int velocidad);
	void MoverDerecha(int velocidad);
	Sprite* GetNaveObjeto();
	void setVisible(bool visible);
	bool Colision(Nave * nave, TipoColision tipoColision);
	void crearNuevo(int pos);
	//////////// OPENGL
	GLuint GetTexture();

	///////////////////
};
#endif