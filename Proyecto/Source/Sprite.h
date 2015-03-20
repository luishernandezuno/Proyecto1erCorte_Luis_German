#ifndef _SPRITE_H_
#define _SPRITE_H_
#include <SDL.h>
#include "SpriteDeF.h"

class Sprite{
	SDL_Surface * imagen;//La imagen a manipular
	SDL_Surface * screen;//la pantalla
	SpriteDef spriteDef;//definiciones de sprite
public:
	Sprite(SDL_Surface * screen);//constructor recibe la pantalla
	~Sprite();
	void cargarimagen (char * nombre);
	/*void PintarModulo(int nombre, int x, int y, int w, int h);*/
	void PintarModulo(int nombre, int x, int y);
	int WidthModule(int module);
	int HeightModule(int module);
};


#endif