//Se todo o baralho estiver na lista de cartas finais, fim de jogo
char verificaFimDeJogo(Lista_C *lc)
{
	Lista *aux_lista;
	int total = 0;
	
	aux_lista = lc->prox->prox->l;
	while(aux_lista != NULL)
	{
		total += aux_lista->num_cartas;
		aux_lista = aux_lista->prox;
	}

	return total == BARALHO;
}

//Controla virar uma carta do monte
void virarCarta(Lista_C **lc,int *pontos)
{
	if (isEmptyPilha((*lc)->l->cartas)) //Se o monte estiver vazio, reempilha as cartas viradas
	{
		if (!isEmptyPilha((*lc)->prox->l->cartas)) //Se o monte de viradas não estiver vazio
		{
			reempilhaMonte(&(*lc)->prox->l->cartas, &(*lc)->l->cartas);
			(*lc)->l->num_cartas = (*lc)->prox->l->num_cartas;
			(*lc)->prox->l->num_cartas = 0;
			
			//Se percorrer o baralho todo, perde 100 pontos;
			if (*pontos < 100)
				*pontos = 0;
			else
				*pontos -= 100;
		}
	}
	else
		retiraCartaMonte(lc); //Retira uma carta do monte e coloca na pilha de cartas viradas
}

//Controla seleção de cartas para troca
void controlaSelecao(Lista_C **lc,char tecla_destino,char tecla_origem,int *cont,int *pontos,ALLEGRO_BITMAP *cartas)
{
	if (tecla_destino != 27) //Se tecla igual a esc, cancela seleção
	{
		//Seleção de múltiplas cartas
		if (tecla_origem == tecla_destino && tecla_origem >= 49 && tecla_origem <= 55)
		{
			*cont += 1; //++ não funciona
			desenhaSelecionada(cartas, *lc,cont,tecla_destino);
		}
		else
		{
			//Se for uma seleção múltipla, o destino deve ser uma pilha temporária
			if(*cont > 0)
			{
				if(tecla_destino >= 49 && tecla_destino <= 55)
				{
					controlaTrocaCartas(lc,tecla_origem,tecla_destino,*cont,pontos);
					*cont = 0;
				}
			}
			else				
			{
				controlaTrocaCartas(lc, tecla_origem, tecla_destino,*cont,pontos);
				*cont = 0;
			}
		}
	}
}

//Recebe a entrada e processa a ação
void controleEntrada(Lista_C **lc,ALLEGRO_EVENT_QUEUE *filaEventos,ALLEGRO_DISPLAY *janela,ALLEGRO_BITMAP *fundo,ALLEGRO_BITMAP *cartas,ALLEGRO_FONT *fonte)
{
	int fim = 0;
	int cont = 0;
	int pontos = 0;
	char tecla_origem;
	char tecla_destino;
	char tecla_aux;
	ALLEGRO_EVENT evento;

	al_register_event_source(filaEventos, al_get_display_event_source(janela));
	al_register_event_source(filaEventos, al_get_keyboard_event_source());

	while(!fim)
	{
		al_wait_for_event(filaEventos, &evento);

		if(evento.type == ALLEGRO_EVENT_KEY_CHAR)
		{
			//Se a tecla pressionada for *
			if(evento.keyboard.unichar == '*')
			{
				virarCarta(lc,&pontos);
				desenhaTela(fundo, cartas, fonte, janela, *lc,pontos);
				desenhaTeste(*lc);
			}
			else
			{
				if(cont > 0)
					tecla_aux = tecla_origem; //Auxiliar para não perder a tecla anterior em seleção múltipla
				
				tecla_origem = evento.keyboard.unichar;
				if(tecla_origem >= 49 && tecla_origem <= 55 || tecla_origem >= 97 && tecla_origem <= 100 || tecla_origem == 43) //Se for uma tecla válida
				{
					if(cont == 0) //Se não há seleção múltipla, espera o destino
					{
						desenhaSelecionada(cartas,*lc,&cont,tecla_origem); //Desenha a carta selecionada sombreada
					
						do
							al_wait_for_event(filaEventos, &evento);
						while(evento.type != ALLEGRO_EVENT_KEY_CHAR); //Enquanto o evento for diferente de pressionar uma tecla
						
						tecla_destino = evento.keyboard.unichar;
					}
					else
					{
						tecla_destino = tecla_origem; //Senão a tecla pressionada como o origem será leva em conta como destino, pois a origem já foi pressionada na iteração anterior
						tecla_origem = tecla_aux; 
					}
					
					controlaSelecao(lc,tecla_destino,tecla_origem,&cont,&pontos,cartas);
					
					if(cont == 0) //Se não há seleção múltipla, atualiza a tela
						desenhaTela(fundo,cartas,fonte,janela,*lc,pontos);
				}
			}

			if (verificaFimDeJogo(*lc))
			{
				recebeHighScore(fonte, evento, filaEventos, pontos);
				fim = 1;
			}
		}
		else
			if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				fim = 1;
	}

	exibeHighScore(fonte);
}