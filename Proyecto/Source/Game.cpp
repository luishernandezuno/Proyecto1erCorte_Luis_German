#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include "Config.h"
#include <SDL.h>

CGame::CGame(){
	estadoJuego = ESTADO_INICIANDO;
	tiempoFrameInicial = CERO;
	tick = CERO;
	atexit(SDL_Quit);
}

void CGame::IniciandoVideo()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Video Init: ", (const char *)SDL_GetError(), NULL);
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize SDL: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	
	window = SDL_CreateWindow(VERSION, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH_SCREEN, HEIGHT_SCREEN, SDL_WINDOW_OPENGL);
	
	if (!window) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Windows OpenGL Init: ", (const char *)SDL_GetError(), NULL);
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create OpenGL window: %s\n", SDL_GetError());
		SDL_Quit();
		exit(2);
	}
	
	gContext = SDL_GL_CreateContext(window);

	if (!gContext) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "OpenGL Context Init: ", (const char *)SDL_GetError(), NULL);
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create OpenGL context: %s\n", SDL_GetError());
		SDL_Quit();
		exit(2);
	}
	openGlImplement.setSDLWindow(window);
}

void CGame::CargandoObjetos()
{
	menuFondo = new Sprite(&openGlImplement, "Menu", 0, 0);
	textoTitulo = new Sprite(&openGlImplement, "Texto_Titulo", 0, 0);
	textoNombre = new Sprite(&openGlImplement, "Texto_Nombre", 0, 0);
	textoOpcion1 = new Sprite(&openGlImplement, "Texto_Opcion1", 0, 0);
	textoOpcion2 = new Sprite(&openGlImplement, "Texto_Opcion2", 0, 0);
	textoOpcion1Sel = new Sprite(&openGlImplement, "Texto_Opcion1Sel", 0, 0);
	textoOpcion2Sel = new Sprite(&openGlImplement, "Texto_Opcion2Sel", 0, 0);
	nave = new Nave(&openGlImplement, "MiNave", (WIDTH_SCREEN / 2), (HEIGHT_SCREEN - 80), NAVE_PROPIA);
	jugandoFondo = new Sprite(&openGlImplement, "Jugando", 0, 0);
	ganasteFondo = new Sprite(&openGlImplement, "Ganaste", 0, 0);
	perdisteFondo = new Sprite(&openGlImplement, "Perdiste", 0, 0);

	for (int i = 0; i < MAXIMO_DE_ENEMIGOS; i++)
	{
		enemigoArreglo[i] = new Nave(&openGlImplement, "Enemigo", i * 2, 0, NAVE_ENEMIGA);
		enemigoArreglo[i]->GetNaveObjeto()->SetAutoMovimiento(false);
		enemigoArreglo[i]->GetNaveObjeto()->SetPasoLimite(4);
	}

	opcionSeleccionada = MENU_OPCION1;
}
// Con esta función eliminaremos todos los elementos en pantalla
void CGame::Finalize(){
	delete(nave);
	
	SDL_Quit();
}

bool CGame::Start()
{
	// Esta variable nos ayudara a controlar la salida del juego...
	int salirJuego = false;

	while (salirJuego == false){
		openGlImplement.DrawStart();
		keys = (Uint8*)SDL_GetKeyboardState(NULL);
		//Maquina de estados
		switch (estadoJuego){
		case Estado::ESTADO_INICIANDO:
			IniciandoVideo();
			openGlImplement.InitGL();
			openGlImplement.InitShaders();
			CargandoObjetos();
			InicializandoStage();
			estadoJuego = Estado::ESTADO_MENU;
			break;
		case Estado::ESTADO_MENU:
			MenuActualizar();
			MenuPintar();
			break;
		case Estado::ESTADO_PRE_JUGANDO:
			nivelActual = CERO;
			vida = UNO;
			enemigosEliminados = CERO;
			estadoJuego = ESTADO_JUGANDO;
			juegoGanado = false;
			IniciarEnemigo();
			IniciarNave();
			break;
		case Estado::ESTADO_JUGANDO:
			JugandoActualizar();
			JugandoPintar();
			break;
		case Estado::ESTADO_FINALIZANDO:
			estadoJuego = Estado::ESTADO_ESPERANDO;
			salirJuego = false;
			openGlImplement.QuitShaders();
			SDL_Quit();

			break;
		case Estado::ESTADO_TERMINANDO:
			TerminadoPintar();
			TerminadoActualizar();
			break;

		case Estado::ESTADO_ESPERANDO:

			break;
		};

		openGlImplement.DrawEnd();

		while (SDL_PollEvent(&event))//Aqui sdl creara una lista de eventos ocurridos
		{
			if (event.type == SDL_QUIT) { salirJuego = true; } //si se detecta una salida del sdl o.....
			if (event.type == SDL_KEYDOWN) {}
		}

		//Calculando fps
		tiempoFrameFinal = SDL_GetTicks();
		while (tiempoFrameFinal < (tiempoFrameInicial + FPS_DELAY))
		{
			tiempoFrameFinal = SDL_GetTicks();
			SDL_Delay(1);
		}

		tiempoFrameInicial = tiempoFrameFinal;
		tick++;


	}
	return true;
}

bool CGame::LimitePantalla(Sprite*objeto, int bandera)
{
	if (bandera & BORDE_IZQUIERDO)
		if (objeto->GetX() <= 0)
			return true;
	if (bandera & BORDE_SUPERIOR)
		if (objeto->GetY() <= 0)
			return true;

	if (bandera & BORDE_DERECHO)
		if (objeto->GetX() >= (WIDTH_SCREEN - objeto->GetW()))
			return true;
	if (bandera & BORDE_INFERIOR)
		if (objeto->GetY() >= HEIGHT_SCREEN - objeto->GetH())
			return true;
	return false;

}//Termina LimitePantalla

void CGame::MoverEnemigo(){

	for (int i = 0; i < nivel[nivelActual].Enemigos_VisiblesAlMismoTiempo; i++)
	{
		if (enemigoArreglo[i]->GetNaveObjeto()->ObtenerPasoActual() == 0)
			if (!LimitePantalla(enemigoArreglo[i]->GetNaveObjeto(), BORDE_DERECHO))
				enemigoArreglo[i]->GetNaveObjeto()->MoverLados(nivel[nivelActual].Enemigo_Velocidad);//Derecha
			else{
				enemigoArreglo[i]->GetNaveObjeto()->IncrementarPasoActual();
			}//fin else derecho

			if (enemigoArreglo[i]->GetNaveObjeto()->ObtenerPasoActual() == 1)
				if (!LimitePantalla(enemigoArreglo[i]->GetNaveObjeto(), BORDE_INFERIOR))
					enemigoArreglo[i]->GetNaveObjeto()->MoverArribaAbajo(nivel[nivelActual].Enemigo_Velocidad);//Abajo
				else{
					enemigoArreglo[i]->GetNaveObjeto()->IncrementarPasoActual();
				}//Fn else inferior

				if (enemigoArreglo[i]->GetNaveObjeto()->ObtenerPasoActual() == 2)
					if (!LimitePantalla(enemigoArreglo[i]->GetNaveObjeto(), BORDE_IZQUIERDO))
						enemigoArreglo[i]->GetNaveObjeto()->MoverLados(-nivel[nivelActual].Enemigo_Velocidad);//Izquierda
					else{
						enemigoArreglo[i]->GetNaveObjeto()->IncrementarPasoActual();
					}//fin else izquierda

					if (enemigoArreglo[i]->GetNaveObjeto()->ObtenerPasoActual() == 3)
						if (!LimitePantalla(enemigoArreglo[i]->GetNaveObjeto(), BORDE_SUPERIOR))
							enemigoArreglo[i]->GetNaveObjeto()->MoverArribaAbajo(-nivel[nivelActual].Enemigo_Velocidad);//Arriba
						else{
							enemigoArreglo[i]->GetNaveObjeto()->IncrementarPasoActual();
						}//fin else arriba
	}
}//Termina MoverEnemigo

void CGame::JugandoPintar(){
	jugandoFondo->Draw();
	////////////////////////////////////////
	//////// CONTROL DE COLISIONES /////////
	for (int i = 0; i < nivel[nivelActual].Enemigos_VisiblesAlMismoTiempo; i++)
	{
		
		if (enemigoArreglo[i]->Colision(nave, Nave::TipoColision::NAVE) || enemigoArreglo[i]->Colision(nave, Nave::TipoColision::BALA))//Nave vs Nave Enemigo
			vida--;
		if (nave->Colision(enemigoArreglo[i], Nave::TipoColision::BALA)){//Nave vs Naves Bala
			enemigoArreglo[i]->setVisible(false);
			enemigosEliminados++;
			if (enemigosEliminados < nivel[nivelActual].Enemigo_EliminarPorNivel)
			{
				enemigoArreglo[i]->crearNuevo(rand() % (WIDTH_SCREEN - 64));
			}
		}
	}
	/////////////////////////////////////////
	if (enemigosEliminados >= nivel[nivelActual].Enemigo_EliminarPorNivel)
	{
		if (nivelActual < (MAXIMO_DE_NIVELES-1))
			nivelActual++;
		else{
			juegoGanado = true;
			estadoJuego = ESTADO_TERMINANDO;
		}
	}
	if (vida <= CERO)
		estadoJuego = ESTADO_TERMINANDO;

	nave->Pintar();
	for (int i = 0; i < nivel[nivelActual].Enemigos_VisiblesAlMismoTiempo; i++)
	{
		enemigoArreglo[i]->Pintar();
		enemigoArreglo[i]->AutoDisparar(nivel[nivelActual].Enemigo_VelocidadBala);
	}
}

void CGame::JugandoActualizar(){
	keys = (Uint8 *)SDL_GetKeyboardState(NULL);

	for (int i = 0; i < nivel[nivelActual].Enemigos_VisiblesAlMismoTiempo; i++)
	{
		enemigoArreglo[i]->GetNaveObjeto()->Actualizar();
	}
	MoverEnemigo();

	if (keys[SDL_SCANCODE_UP])
	{
		if (!LimitePantalla(nave->GetNaveObjeto(), BORDE_SUPERIOR))
			nave->MoverArriba(nivel[nivelActual].Nave_Velocidad);
	}
	if (keys[SDL_SCANCODE_DOWN])
	{
		if (!LimitePantalla(nave->GetNaveObjeto(), BORDE_INFERIOR))
			nave->MoverAbajo(nivel[nivelActual].Nave_Velocidad);
	}

	if (keys[SDL_SCANCODE_LEFT])
	{
		if (!LimitePantalla(nave->GetNaveObjeto(), BORDE_IZQUIERDO))
			nave->MoverIzquierda(nivel[nivelActual].Nave_Velocidad);
	}
	if (keys[SDL_SCANCODE_RIGHT])
	{
		if (!LimitePantalla(nave->GetNaveObjeto(), BORDE_DERECHO))
			nave->MoverDerecha(nivel[nivelActual].Nave_Velocidad);
	}

	if (keys[SDL_SCANCODE_ESCAPE])
	{
		estadoJuego = Estado::ESTADO_MENU;
	}
	if (keys[SDL_SCANCODE_SPACE])
	{
		nave->Disparar(nivel[nivelActual].Nave_BalasMaximas);
	}
	
	if (keys[SDL_SCANCODE_C]){//nuestra bala / nave enemigo
		int enemigoAEliminar = rand() % nivel[nivelActual].Enemigos_VisiblesAlMismoTiempo;
		//enemigoArreglo[enemigoAEliminar]->simularColision(true);
	}

	if (keys[SDL_SCANCODE_V]){//nuestra nave / nave enemigo

	}
}

void CGame::MenuActualizar()
{
		if (keys[SDL_SCANCODE_UP])
		{
			opcionSeleccionada = MENU_OPCION1;
		}

		if (keys[SDL_SCANCODE_DOWN])
		{
			opcionSeleccionada = MENU_OPCION2;
		}

		if (keys[SDL_SCANCODE_RETURN])
		{
			if (opcionSeleccionada == MENU_OPCION1)
			{
				estadoJuego = Estado::ESTADO_PRE_JUGANDO;
			}

			if (opcionSeleccionada == MENU_OPCION2)
			{
				estadoJuego = Estado::ESTADO_FINALIZANDO;
			}
		}// SDL_SCANCODE__return 
}

void CGame::MenuPintar()
{
	menuFondo->Draw();
	textoTitulo->TranslateXYDraw(WIDTH_SCREEN / 8, 0);

	textoNombre->TranslateXYZ( WIDTH_SCREEN / 3, 450, 0.f);//570
//	textoNombre->ScaleXYZ(30.f,30.f,30.f);
//	textoNombre->RotateXYZ(0.f, 0.f, 0.f);
	textoNombre->Draw();

	textoOpcion1->TranslateXYDraw(320, 220);
	textoOpcion2->TranslateXYDraw(320, 220 + 30);

	if (opcionSeleccionada == MENU_OPCION1)
		textoOpcion1Sel->TranslateXYDraw(320, 220);
	else
		textoOpcion2Sel->TranslateXYDraw(320, 220 + 30);

}//void	

void CGame::IniciarEnemigo(){
	for (int i = 0; i < nivel[nivelActual].Enemigos_VisiblesAlMismoTiempo; i++)
		enemigoArreglo[i]->crearNuevo(rand() % (WIDTH_SCREEN - 64));
}

void CGame::IniciarNave(){
	nave->crearNuevo(WIDTH_SCREEN / 2);
}

void CGame::TerminadoPintar(){
	if (juegoGanado)
		ganasteFondo->Draw();
	else
		perdisteFondo->Draw();
}

void CGame::TerminadoActualizar(){
	if (keys[SDL_SCANCODE_RETURN]){
		estadoJuego = Estado::ESTADO_MENU;
	}
}

