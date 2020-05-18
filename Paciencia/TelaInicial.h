void desenhaTelaInicial(ALLEGRO_BITMAP *fundo, ALLEGRO_FONT *fonte)
{
	//Carrega fonte e desenha título
	fonte = al_load_font("LCALLIG.ttf", 60, 0);
	al_draw_bitmap(fundo, 0, 0, 0);
	al_draw_text(fonte, al_map_rgb(255, 255, 255), LARGURA / 2, 100, ALLEGRO_ALIGN_CENTRE, "Leozinho's Solitaire");
	
	//Carrega fonte e desenha menu
	fonte = al_load_font("GILLUBCD.ttf", 40, 0);
	al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA / 2, 400, ALLEGRO_ALIGN_CENTRE, "1 - Start Game");
	al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA / 2, 450, ALLEGRO_ALIGN_CENTRE, "2 - High Scores");
	al_flip_display();
}

void telaInicial(ALLEGRO_EVENT_QUEUE *filaEventos, ALLEGRO_DISPLAY *janela)
{
	int fim = 0;
	ALLEGRO_EVENT evento;
	ALLEGRO_BITMAP *fundo;
	ALLEGRO_FONT *fonte = NULL;

	fundo = al_load_bitmap("FundoTelaInicial.jpg");
	
	desenhaTelaInicial(fundo, fonte);

	al_register_event_source(filaEventos, al_get_keyboard_event_source());

	do
	{
		al_wait_for_event(filaEventos, &evento);

		if (evento.type == ALLEGRO_EVENT_KEY_CHAR)
		{
			if (evento.keyboard.unichar == '1')
			{
				fim = 1; //Se apertar 1, fecha a tela inicial e começa o jogo
			}
			else
			if (evento.keyboard.unichar == '2')
			{
				exibeHighScore(fonte); //Se apertar 2, chama a função para mostrar o high score
				desenhaTelaInicial(fundo, fonte);
			}
		}


	} while (fim == 0);

	al_destroy_bitmap(fundo);
	al_destroy_font(fonte);
}