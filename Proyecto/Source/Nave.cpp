#include "Nave.h"
#include "Config.h"
#include <stdlib.h>

Nave::Nave(OpenGlImplement* openGlImplement, char*rutaImagen, int x, int y, int tipoNave)
{
	nave = new Sprite(openGlImplement, rutaImagen, x, y);
	for (int i = 0; i < MAXIMO_DE_BALAS; i++)
	{
		bala[i] = new Sprite(openGlImplement, "Balas", 0, 0);
		bala[i]->SetVisible(false);
	}
	balaVisible = 0;
	visible = true;
	colision = false;
	this->tipoNave = tipoNave;
}

void Nave::crearNuevo(int pos)
{
	balaVisible = 0;
	visible = true;
	colision = false;
	for (int i = 0; i < MAXIMO_DE_BALAS; i++)
	{
		bala[i]->SetVisible(false);
		bala[i]->TranslateXY(-5, 0);
	}
	if (tipoNave == NAVE_PROPIA)
		nave->TranslateXY(pos, HEIGHT_SCREEN - 64);
	if (tipoNave == NAVE_ENEMIGA)
		nave->TranslateXY(pos, 0);
}

void Nave::Disparar(int balas)
{
	if (visible)
	{
		bala[balaVisible]->SetVisible(true);
		switch (tipoNave)
		{
		case NAVE_PROPIA:
			bala[balaVisible]->TranslateXY(nave->GetX() + nave->GetW() / 2, nave->GetY());
			break;

		case NAVE_ENEMIGA:
			bala[balaVisible]->TranslateXY(nave->GetX() + nave->GetW() / 2, nave->GetY() + nave->GetH());
			break;
		}
		balaVisible++;

		if (balaVisible >= balas)
			balaVisible = 0;
	}
}

void Nave::Draw()
{
	if (visible){
		nave->Draw();
		for (int i = 0; i < MAXIMO_DE_BALAS; i++)
		{
			bala[i]->Draw();
			switch (tipoNave)
			{
			case NAVE_PROPIA:
				bala[i]->MoverArribaAbajo(-10);
				break;

			case NAVE_ENEMIGA:
				bala[i]->MoverArribaAbajo(10);
				break;
			}
		}
	}
}

void Nave::MoverAbajo(int velocidad)
{
	nave->MoverArribaAbajo(velocidad);
}
void Nave::MoverArriba(int velocidad)
{
	nave->MoverArribaAbajo(-velocidad);
}
void Nave::MoverDerecha(int velocidad)
{
	nave->MoverLados(velocidad);
}
void Nave::MoverIzquierda(int velocidad)
{
	nave->MoverLados(-velocidad);
}
Sprite*Nave::GetNaveObjeto()
{
	return nave;
}

void Nave::AutoDisparar(int balas)
{
	if ((rand()%100)<2)
	{
		Disparar(balas);
	}
}

void Nave::setVisible(bool visible)
{
	this->visible = visible;
}

bool Nave::Colision(Nave * nave, TipoColision tipoColision){
	int x_o, y_o, w_o, h_o;
	int x, y, w, h;
	
	switch (tipoColision){
	case TipoColision::NAVE:
		x = GetNaveObjeto()->GetX();
		y = GetNaveObjeto()->GetY();
		w = GetNaveObjeto()->GetW();
		h = GetNaveObjeto()->GetH();
		
		x_o = nave->GetNaveObjeto()->GetX();
		y_o = nave->GetNaveObjeto()->GetY();
		w_o = nave->GetNaveObjeto()->GetW();
		h_o = nave->GetNaveObjeto()->GetH();

		if ((x <= x_o + w_o && 	y <= y_o + h_o) && (x_o <= x + w && y_o <= y + h))
			return true;

		break;
	case TipoColision::BALA:
		x = nave->GetNaveObjeto()->GetX();
		y = nave->GetNaveObjeto()->GetY();
		w = nave->GetNaveObjeto()->GetW();
		h = nave->GetNaveObjeto()->GetH();

		for (int i = 0; i < MAXIMO_DE_BALAS; i++){
			x_o = bala[i]->GetX();
			y_o = bala[i]->GetY();
			w_o = bala[i]->GetW();
			h_o = bala[i]->GetH();

			if ((x <= x_o + w_o && 	y <= y_o + h_o) && (x_o <= x + w && y_o <= y + h))
				return true;
		}
		break;
	}

	return false;

}