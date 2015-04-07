#pragma once
#include "Nave.h"
#include "Stage.h"
#include "Config.h"
#include "SDL_opengl.h"
#include "OpegGLImplement.h"


class CGame
{
public:
	bool Start();
	static CGame instanceGame;

	CGame();
	void Finalize();
	
	enum Estado{
		ESTADO_INICIANDO,
		ESTADO_MENU,
		ESTADO_PRE_JUGANDO,
		ESTADO_JUGANDO,
		ESTADO_TERMINANDO,
		ESTADO_FINALIZANDO,
		ESTADO_ESPERANDO
	};


private:
	void CargandoObjetos();
	void IniciandoVideo();
	void MoverEnemigo();
	void MenuPintar();
	void MenuActualizar();
	void JugandoPintar();
	void JugandoActualizar();
	void TerminadoPintar();
	void TerminadoActualizar();
	bool LimitePantalla(Sprite * objeto, int bandera);
	void InicializandoStage();
	void IniciarEnemigo();
	void IniciarNave();
	
	Uint8 *keys;//Esta variable nos servira para ver si determinadas teclas estan o no pulsadas
	SDL_Event event;//La variable event de tipo evento de SDL nos servira para motorizar
	SDL_Surface *screenBuffer;
	
	///////// NUEVO
	SDL_Window *window;
	SDL_GLContext gContext;
	OpenGlImplement openGlImplement;

	///////////

	int opcionSeleccionada;
	int nivelActual;
	int vida;
	int enemigosEliminados;
	bool juegoGanado;

	Nave *nave;
	Nave *enemigoArreglo[MAXIMO_DE_ENEMIGOS];
	Stage nivel[MAXIMO_DE_NIVELES];
	
	Estado estadoJuego;
	
	int tick;
	int tiempoFrameInicial;
	int tiempoFrameFinal;

	Sprite *menuFondo;//Fondo del menu
	Sprite *textoTitulo;//Textos del juego
	Sprite *textoNombre;//Textos del juego
	Sprite *textoOpcion1;//Textos del juego
	Sprite *textoOpcion2;//Textos del juego
	Sprite *textoOpcion1Sel;//Textos del juego
	Sprite *textoOpcion2Sel;//Textos del juego
	Sprite *jugandoFondo;//Fondo del juego
	Sprite *ganasteFondo;//Fondo cuando ganas
	Sprite *perdisteFondo;//Fondo cuando ganas

};