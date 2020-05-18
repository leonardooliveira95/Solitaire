#define LARGURA 1100
#define ALTURA 900

void controleEntrada(Lista_C **lc,ALLEGRO_EVENT_QUEUE *filaEventos,ALLEGRO_DISPLAY *janela,ALLEGRO_BITMAP *fundo,ALLEGRO_BITMAP *cartas,ALLEGRO_FONT *fonte);
void telaInicial(ALLEGRO_EVENT_QUEUE *filaEventos, ALLEGRO_DISPLAY *janela);


//Função para desenhar pilhas de cartas no console
void desenhaTeste(Lista_C *lc)
{
	int i;
	Lista *aux;

	system("cls");
	
	printf("Viradas:\n");
	show(lc->prox->l->cartas);
	
	printf("\n\nFinais:\n");
	
	aux = lc->prox->prox->l;
	for(i = 0;i < 4; i++)
	{
		printf("\n\n%d\n",i + 1);
		show(aux->cartas);
		aux = aux->prox;
	}
	
	printf("\n\nTemporarias:\n");
	
	aux = lc->prox->prox->prox->l;
	for(i = 0;i < 7; i++)
	{
		printf("\n\n%d\n",i + 1);
		show(aux->cartas);
		aux = aux->prox;
	}
}

//Função para desenhar pontuação
void desenhaPontos(ALLEGRO_FONT *fonte,int pontos)
{
	al_draw_textf(fonte, al_map_rgb(255, 255, 255), 1000, 10, 0, "Pontos: %d",pontos);
}

//Função para ajustar posições em par ou ímpar
void parImpar(Carta *c,int *x,int *y)
{
	//Se o número for ímpar
	if(c->num % 2 != 0)
	{
		while(c->num/2 > *y/196)
			*y += 196;
	}
	else
	{
		//Anda uma posição para frente no sprite sheet se for par
		*x += 146;
		while(c->num/2 > *y/196 + 1) //Enquanto não achar o número, soma pixels na posição x
			*y += 196;
		
	}
}

//Função para ajustar posições de valetes, rainhas ou reis
void posJQK(Carta *c,int *x,int *y)
{
	//Se for K,Q ou J, anda 8 posições para frente no sprite sheet
	*x += 146*8;
	if(c->naipe == 5) // Paus
	{
		if(c->num == 12)
		{
			*x += 146;
		}
		else
		if(c->num == 13)
		{
			*y +=196;
		}
	}
	else
	if(c->naipe == 4) //Ouros
	{
		if(c->num == 11)
		{
			*x += 146;
			*y += 196;
		}
		else
		if(c->num == 12)
		{
			*y += 196*2;
		}
		else
		if(c->num == 13)
		{
			*y +=196*2;
			*x += 146;
		}
	}
	else
	if(c->naipe == 6) //Espadas
	{
		if(c->num == 11)
		{
			*y += 196*3;
		}
		else
		if(c->num == 12)
		{
			*y += 196*3;
			*x += 146;
		}
		else
		if(c->num == 13)
		{
			*y +=196*4;
		}
	}
	else
	if(c->naipe == 3) //Copas
	{
		if(c->num == 11)
		{
			*x += 146;
			*y += 196*4;
		}
		else
		if(c->num == 12)
		{
			*y += 196*5;
		}
		else
		if(c->num == 13)
		{
			*y +=196*5;
			*x += 146;
		}
	}
}

//Realizar cálculo para devolver posição em pixels do sprite sheet onde a carta está localizada
void getPosCarta(Carta *c,int *x,int *y)
{
	if(c->num > 10)
	{
		posJQK(c,x,y);
	}
	else
	{
		//Se o naipe for vermelho, procura entre as 4 primeiras posições
		if(c->cor == 'V')
		{
			//Se for naipe de copas
			if(c->naipe == 3)
			{
				parImpar(c,x,y);
			}
			else
			{
				//Naipe de ouros
				//Anda duas posições a frente no sprite sheet
				*x += 146*2;
				parImpar(c,x,y);
			}
		}
		else
		{
			//Senão, x avança 4 posições a direita (144 sendo a largura da carta)
			*x = 146*4;
			if(c->naipe == 5) //Naipe de paus
			{
				parImpar(c,x,y);
			}
			else
			{
				//Naipe de espadas
				*x += 146 * 2;
				parImpar(c,x,y);
			}
		}
	}
}

//Desenha espaços vazios
void desenhaEspacos(ALLEGRO_BITMAP *cartas)
{
	int i,j,k;
	
	//Desenha os espaços de cartas finais
	for (i = 0, j = 462, k = 602; i < 4; i++, j += 160, k += 160)
		al_draw_rounded_rectangle(j, 35, k, 225, 5, 5, al_map_rgb(255, 255, 255), 1);

	//Desenha os espaços de cartas temporárias
	for (i = 0, j = 25, k = 163; i < 7; i++, j += 150, k += 150)
		al_draw_rounded_rectangle(j, 275, k, 465, 5, 5, al_map_rgb(255, 255, 255), 1);

	//Desenha espaço do monte
	al_draw_rounded_rectangle(27, 35, 165, 220, 5, 5, al_map_rgb(255, 255, 255), 1);

	al_flip_display();
		
}

//Desenha cartas temporárias
void desenhaTemporarias(ALLEGRO_BITMAP *cartas,Lista_C *lc)
{
	int i,j,k,l;
	int x,y;
	
	Lista *aux_lista;
	Pilha *aux_pilha;
	Carta *c;
	
	//Auxiliar para percorrer a lista de cartas temporárias
	aux_lista = lc->prox->prox->prox->l;

	//Desenha as cartas na pilha de temporárias
	for(i = 1,l = 22; i <= 7;i++,l+=150)
	{
		if(!isEmptyPilha(aux_lista->cartas))
		{
			initPilha(&aux_pilha);
			copiaPilha(aux_lista->cartas,&aux_pilha);
			invertePilha(&aux_pilha);

			for(j = 0,k = 272;!isEmptyPilha(aux_pilha); j++,k += 40)
			{
				pop(&aux_pilha,&c);
				if(c->virada == 0)
				{
					x = 0;
					y = 0;
					getPosCarta(c,&x,&y);
					al_draw_bitmap_region(cartas,x,y,144,194,l,k,0); //Bitmap,x_bitmap,y_bitmap_,altura,largura,destino_x,destino_y,flags
				}
				else
					al_draw_bitmap_region(cartas,588,981,144,194,l,k,0);
			}
		}
		aux_lista = aux_lista->prox;
	}
}

//Desenha cartas finais
void desenhaFinais(ALLEGRO_BITMAP *cartas,Lista_C *lc)
{
	int i,l;
	int x,y;

	Lista *aux_lista;

	//Auxiliar para percorer lista de cartas finais
	aux_lista = lc->prox->prox->l;

	//Desenha as cartas na pilha de cartas finais
	for(i = 1,l = 460;i <= 4;i++,l += 160)
	{
		if(!isEmptyPilha(aux_lista->cartas))
		{
			x = 0;
			y = 0;
			getPosCarta(aux_lista->cartas->c,&x,&y);
			al_draw_bitmap_region(cartas,x,y,144,194,l,33,0);
		}

		aux_lista = aux_lista->prox;
	}
}

//Desenha o monte de cartas
void desenhaMonte(ALLEGRO_BITMAP *cartas,Lista_C *lc)
{
	if(!isEmptyPilha(lc->l->cartas))
	{
		//Desenha uma carta virada na posição do monte
		al_draw_bitmap_region(cartas,588,981,144,194,25,31,0);
	}
}

//Desenha a carta virada do monte
void desenhaVirada(ALLEGRO_BITMAP *cartas,Lista_C *lc)
{
	int x,y;

	//Desenha a carta virada
	if(!isEmptyPilha(lc->prox->l->cartas))
	{
		x = 0;
		y = 0;
		getPosCarta(lc->prox->l->cartas->c,&x,&y);
		al_draw_bitmap_region(cartas,x,y,144,194,200,31,0);
	}
}

//Função para desenhar uma carta sombreada
void desenhaSelecionada(ALLEGRO_BITMAP *cartas, Lista_C *lc, int *cont, char tecla)
{
	int i;
	int x, y;
	Lista *aux;
	Pilha *p;

	initLista(&aux);
	initPilha(&p);

	if (tecla >= 49 && tecla <= 55) //Temporárias
	{
		aux = lc->prox->prox->prox->l;

		for (i = 49, x = 22; i < tecla; i++, x += 150) //Percore até chegar na pilha selecionada
			aux = aux->prox;

		y = 272; //Altura da primeira carta temporária
		y += aux->num_cartas > 0 ? (aux->num_cartas - 1) * 40 : 0; //Número de cartas temporárias

		if (*cont < aux->num_cartas)
			for (i = 0; i < *cont; i++) //Percorre verticalmente para cima o número de vezes que a tecla foi pressionada
				y -= 40;
		else
			*cont = 0; //Se a tecla foi apertada mais vezes que o número de cartas da pilha, zera o contador
	}
	else
	if (tecla >= 97 && tecla <= 100) //Finais
	{
		aux = lc->prox->prox->l;

		for (i = 97, x = 460; i < tecla; i++, x += 160)
			aux = aux->prox;

		y = 33; //Altura das cartas finais
	}
	else
	if (tecla == 43) //Viradas
	{
		aux = lc->prox->l;

		x = 200;
		y = 31;
	}

	//Se a posição selecionada não estiver vazia
	if (!isEmptyPilha(aux->cartas))
	{
		p = aux->cartas;
		for (i = 0; i < *cont; i++) //Percorre até a carta selecionada
			p = p->prox;

		//Só desenha se a carta não estiver virada
		if (p->c->virada == 0)
		{
			desenhaTemporarias(cartas, lc); //Desenha as temporárias para "limpar" a carta selecionada anteriormente
			
			//Não é necessário buscar no sprite sheet a posição da carta, já que é somente uma sombra
			if (*cont > 0)
				al_draw_tinted_bitmap_region(cartas, al_map_rgba_f(255, 0, 0, 0.5), 0, 0, 144, 40, x, y, 0); 
			else
				al_draw_tinted_bitmap_region(cartas, al_map_rgba_f(255, 0, 0, 0.5), 0, 0, 144, 194, x, y, 0);
			
			al_flip_display();
		}
		else
			*cont = 0;
	}
	else
	{
		//Se a pilha selecionada estiver vazia, desenha uma sombra no local
		al_draw_tinted_bitmap_region(cartas, al_map_rgba_f(255, 0, 0, 0.5), 0, 0, 144, 194, x, y, 0);
		al_flip_display();
	}
}

//Função para controlar o desenho de toda a tela
void desenhaTela(ALLEGRO_BITMAP *fundo,ALLEGRO_BITMAP *cartas,ALLEGRO_FONT *fonte,ALLEGRO_DISPLAY *janela,Lista_C *lc,int pontos)
{
	//Desenha o fundo
	al_draw_bitmap(fundo, 0, 0, 0);
	
	//Desenha os espaços vazios
	desenhaEspacos(cartas);
	
	//Desenha carta virada na posição do monte
	desenhaMonte(cartas,lc);

	//Desenha carta virada
	desenhaVirada(cartas,lc);

	//Desenha cartas temporárias
	desenhaTemporarias(cartas,lc);	

	//Desenha cartas finais
	desenhaFinais(cartas,lc);

	//Desenha a pontuação
	desenhaPontos(fonte, pontos);

	al_flip_display();
}

//Recupera a resolução atual do desktop
void resolucaoTela(int *w,int *h)
{
	ALLEGRO_MONITOR_INFO info;
	al_get_monitor_info(al_get_num_video_adapters() - 1, &info);

	*w = info.x2 - info.x1;
	*h = info.y2 - info.y1;
}

//Função para inicializações
int inicializa_al()
{
	//Inicializa allegro
	if(!al_init())
	{
		printf("Falha ao inicializar allegro\n");
		return -1;
	}

	//Inicializa add-on de imagens
	if(!al_init_image_addon())
	{
		printf("Falha ao inicializar imagens\n");
		return -1;
	}
	
	//Inicializa add-on de fontes
	al_init_font_addon();

	//Inicializa add-on de fontes true-type
	if(!al_init_ttf_addon())
	{
		printf("Falha ao inicializar add-on fontes true-type\n");
		return -1;
	}

	//Inicializa add-on de primitivas
	if(!al_init_primitives_addon())
	{
		printf("Falha ao inicializar primitivas\n");
		return -1;
	}
	
	//Inicializa o teclado
	if (!al_install_keyboard())
	{
		printf("Falha ao inicializar o teclado\n");
		return -1;
	}

	return 0;
}

char carregaFonte(ALLEGRO_FONT **fonte)
{
	*fonte = al_load_font("segoeuil.ttf",15,0);
	if(!*fonte)
	{
		printf("Falha ao inicializar fonte\n");
		return -1;
	}

	return 0;
}

char criaFilaEventos(ALLEGRO_EVENT_QUEUE **filaEventos)
{
	*filaEventos = al_create_event_queue();
    if (!*filaEventos)
    {
        printf("Falha ao criar fila de eventos\n");
        return -1;
    }

	return 0;
}

char criaJanela(ALLEGRO_DISPLAY **janela)
{
	//int w,h;
	//resolucaoTela(&w, &h);
	//al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	*janela = al_create_display(LARGURA,ALTURA);
	if(!*janela)
	{
		printf("Falha ao criar a janela\n");
		return -1;
	}

	//Muda o título da janela
	al_set_window_title(*janela,"Leozinho's Solitaire");

	return 0;
}

char carregaBitmaps(ALLEGRO_BITMAP **fundo,ALLEGRO_BITMAP **cartas)
{
	//Carrega bitmap de fundo
	*fundo = al_load_bitmap("Background.jpg");
	if (!*fundo)
	{
		printf("Falha ao carregar bitmap 'fundo'");
		return -1;
	}

	*cartas = al_load_bitmap("Baralho.png");
	if(!*cartas)
	{
		printf("Falha ao carregar bitmap 'cartas'");
		return -1;
	}

	al_convert_mask_to_alpha(*cartas,al_map_rgb(18,18,254));

	return 0;
}

//Carrega arquivos e inicializa funções do allegro
int carregaJogo(Lista_C **lc)
{
	ALLEGRO_DISPLAY *janela = NULL;
	ALLEGRO_FONT *fonte = NULL;
	ALLEGRO_EVENT_QUEUE *filaEventos = NULL;
	ALLEGRO_BITMAP *fundo = NULL;
	ALLEGRO_BITMAP *cartas = NULL;

	//Função para inicializações em geral
	if(inicializa_al() == -1)
		return -1;
	
	//Carrega fonte
	if(carregaFonte(&fonte) == -1)
		return -1;
	
	//Cria fila de eventos
	if(criaFilaEventos(&filaEventos) == -1)
		return -1;
	
	//Cria a janela
	if(criaJanela(&janela) == -1)
		return -1;
	
	//Carrega os bitmaps
	if(carregaBitmaps(&fundo,&cartas) == -1)
		return -1;
	
	//Chama tela Inicial
	telaInicial(filaEventos, janela);
	
	//Desenha a tela
	desenhaTela(fundo,cartas,fonte,janela,*lc,0);
	
	//Chama função que controla o jogo
	controleEntrada(lc, filaEventos, janela,fundo,cartas,fonte);

	//Destrói os elementos carregados
	al_destroy_display(janela);
	al_destroy_font(fonte);
	al_destroy_event_queue(filaEventos);
	al_destroy_bitmap(fundo);
	al_destroy_bitmap(cartas);

	return 0;
}