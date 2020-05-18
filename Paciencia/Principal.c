#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>

#include "Pilha.h"
#include "Lista.h"
#include "ListaControle.h"
#include "ConstrucaoEstrutura.h"
#include "TrocaCartas.h"
#include "Grafico.h"
#include "HighScores.h"
#include "ControleJogo.h"
#include "TelaInicial.h"


int main ()
{
	Lista_C *lc;
	
	srand((unsigned int)time(NULL));
	
	initListaControle(&lc);
	montaEstrutura(&lc);
	carregaJogo(&lc);
	limpaMemoria(&lc);
}
